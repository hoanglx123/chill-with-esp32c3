#!/usr/bin/env python3
import cv2
import os
import argparse
import sys
import shutil
import numpy as np

from string import Template
from pathlib import Path
from enum import Enum

FRAME_CONTENT_TEMPLATE = Template("""
#pragma once

// Auto-generated bitmaps for SSD1306                       
namespace $namespace {

/************************************/
/* Buffer list                      */
/************************************/                                  
$buffer_list
                                            
/************************************/
/* All buffer                       */
/************************************/  
$all_buffer_list
                                                                    
} /* End of namespace */
                        
""")

BUFFER_TEMPLATE = Template("""// Frame: $name ($width x $height) ~ $bytes bytes
static const uint8_t $name[] PROGMEM = {
$buffer
};
""")

ALL_BUFFER_LIST_TEMPLATE = Template("""
// List of all frame arrays
static const FRAME_INFO_t all_frames[] PROGMEM = {
    $all_buffer_list
};
""")

class CompressionType(Enum):
    NONE = 0
    RUN_LENGTH_ENCODING = 1  # Run-length encoding
    DELTA_ENCODING = 2      # Delta encoding (not implemented)

# Custom parser to show full help on error
class SmartFormatter(argparse.ArgumentDefaultsHelpFormatter):
    def error(self, message):
        sys.stderr.write(f"❌ Error: {message}\n\n")
        self.print_help()
        sys.exit(2)

class CommandHelper:
    # ------------------------------
    # Frame processing
    # ------------------------------
    @staticmethod
    def process_frame(frame, width, height, threshold, invert=False):
        """Resize, grayscale, and binarize one frame."""
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        resized = cv2.resize(gray, (width, height), interpolation=cv2.INTER_NEAREST_EXACT)
        _, binary = cv2.threshold(resized, threshold, 255, cv2.THRESH_BINARY)
        if invert:
            binary = cv2.bitwise_not(binary)
        return binary

    @staticmethod 
    def frame_to_array(binary, invert=False):
        # SSD1306 always uses the horizontal packing function
        return CommandHelper.frame_to_array_horizontal(binary, invert)

    @staticmethod
    def frame_to_array_vertical(binary, invert=False):
        """
        Packs the binary image into bytes horizontally (row-major, SSD1306 vertical addressing).
        Each byte represents 8 vertical pixels in a row.
        """
        height, width = binary.shape
        if height % 8 != 0:
            raise ValueError("Height must be divisible by 8 (for vertical pages).")

        arr = []
        for page in range(height // 8):
            for x in range(width):
                byte = 0
                for bit in range(8):
                    y = page * 8 + bit
                    pixel = binary[y, x]
                    bit_val = 1 if (pixel > 0) else 0
                    if invert:
                        bit_val = 1 - bit_val
                    byte |= (bit_val << bit)
                arr.append(byte)
        return arr

    def frame_to_array_horizontal(binary, invert=False):
        """
        Packs the binary image into bytes horizontally (row-major).
        Each byte represents 8 horizontal pixels in a row.
        """
        height, width = binary.shape
        if width % 8 != 0:
            raise ValueError("Width must be divisible by 8 (for horizontal packing).")

        arr = []
        # Iterate through each row
        for y in range(height):
            # Iterate through the row in 8-pixel chunks
            for x_start in range(0, width, 8):
                byte = 0
                # Pack 8 horizontal pixels into one byte
                for bit in range(8):
                    x = x_start + bit
                    pixel = binary[y, x]
                    bit_val = 1 if (pixel > 0) else 0
                    if invert:
                        bit_val = 1 - bit_val
                    byte |= (bit_val << (7 - bit))
                arr.append(byte)
        return arr

    # ------------------------------
    # Saving
    # ------------------------------
    @staticmethod
    def save_frame_png(binary, out_path, frame_id):
        filename = os.path.join(out_path, f"frame_{frame_id:05d}.png")
        cv2.imwrite(filename, binary)
        return filename

    @staticmethod
    def generate_buffer_array(name, arr, width, height):
        content = ""
        for i, val in enumerate(arr):
            if i % 16 == 0:
                content += "    "
            content += f"0x{val:02X}, "
            if (i + 1) % 16 == 0:
                content += "\n"

        content = BUFFER_TEMPLATE.substitute(
            name=name,
            buffer=content,
            width=width,
            height=height,
            bytes=len(arr)
        )

        return content.rstrip("\n")

    @staticmethod
    def generate_all_list(frame_names):
        all_buffer_list = []
        for frame in frame_names:
            all_buffer_list.append(f"{{ {frame[1]}, {frame[0]} }},\n")

        # Generate content
        content = ALL_BUFFER_LIST_TEMPLATE.substitute(
            all_buffer_list="\t".join(all_buffer_list)
        )

        return content.rstrip("\n")

    @staticmethod
    def compressWithRunLengthEncoding(arr):
        if not arr:
            return []

        compressed = []
        prev = arr[0]
        count = 1

        for val in arr[1:]:
            if val == prev and count < 255:
                count += 1
            else:
                compressed.append((count, prev))
                prev = val
                count = 1
        compressed.append((count, prev))

        # Flatten the list
        flat_compressed = []
        for cnt, val in compressed:
            flat_compressed.extend([cnt, val])
        return flat_compressed
    
    @staticmethod
    def compressWithDeltaEncoding(current_frame, previous_frame):
        if not current_frame or not previous_frame or len(current_frame) != len(previous_frame):
            return []

        compressed = []
        for index in range(len(current_frame)):
            if current_frame[index] != previous_frame[index]:
                compressed.extend([index & 0xFF, (index >> 8) & 0xFF, current_frame[index]])

        return compressed

    @staticmethod
    def save_file(file_name, out_path, buffer_array, buffer_list):
        # random_name = ""
        # for i in range(5):
        #     random_name += chr(ord('a') + np.random.randint(0, 26))

        # c_file = Path(out_path) / f"frames_auto_generated_{random_name}.h"
        c_file = Path(out_path) / f"frames_auto_generated_{file_name}.h"

        # Generate content
        content = FRAME_CONTENT_TEMPLATE.substitute(
            namespace= f"frame_{file_name}",
            buffer_list="\n\n".join(buffer_array),
            all_buffer_list=buffer_list,
        )

        with open(c_file, "w") as f:
            f.write(content)

    # ------------------------------
    # Main extractor
    # ------------------------------
    @staticmethod
    def extract_frames(video_path, out_path, fps, width, height, threshold, to_array, preview, invert, is_compress):
        cap = cv2.VideoCapture(video_path)
        if not cap.isOpened():
            print(f"❌ Error: Cannot open video {video_path}")
            return

        # Clean output folder
        if os.path.exists(out_path):
            shutil.rmtree(out_path)
        os.makedirs(out_path, exist_ok=True)

        # Get file name
        file_name = os.path.splitext(os.path.basename(video_path))[0]

        video_fps = cap.get(cv2.CAP_PROP_FPS) or fps
        frame_interval = max(int(video_fps // fps), 1)

        frame_count, saved_count = 0, 0
        frame_names = []
        buffer_array = []
        current_arr = []
        previous_arr = []

        while True:
            ret, frame = cap.read()
            if not ret:
                break

            if frame_count % frame_interval == 0:
                binary = CommandHelper.process_frame(frame, width, height, threshold, invert)

                if preview:
                    cv2.imshow("Preview (press q to quit)", binary)
                    if cv2.waitKey(30) & 0xFF == ord("q"):
                        break

                # Save PNG
                CommandHelper.save_frame_png(binary, out_path, saved_count)

                if to_array:
                    arr = CommandHelper.frame_to_array(binary, invert)

                    # Compress with RLE algorithm if needed
                    if is_compress:
                        # Compress with delta encoding if not the first frame
                        if saved_count > 0:
                            # Compress with delta encoding
                            current_arr = arr
                            arr = CommandHelper.compressWithDeltaEncoding(current_arr, previous_arr)
                            # Store current as previous for next frame
                            previous_arr = current_arr

                        # Compress with run-length encoding if first frame
                        else:
                            previous_arr = arr
                            arr = CommandHelper.compressWithRunLengthEncoding(arr)

                    # Save as C array
                    array_name = f"frame_{saved_count:05d}_bitmap"
                    buffer_array.append(CommandHelper.generate_buffer_array(array_name, arr, width, height))
                    frame_names.append((array_name, len(arr)))

                saved_count += 1

            frame_count += 1

        # Save file
        CommandHelper.save_file(file_name, out_path, buffer_array, CommandHelper.generate_all_list(frame_names))

        cap.release()
        if preview:
            cv2.destroyAllWindows()

        print(f"✅ Done: {saved_count} frames processed in '{out_path}'")

    # ------------------------------
    # CLI options
    # ------------------------------
    @staticmethod
    def create_options():
        parser = argparse.ArgumentParser(
            description="Convert video frames to SSD1306-ready arrays",
            formatter_class=SmartFormatter
        )
        parser.add_argument("video_path", help="Path to input video")
        parser.add_argument("out_path", help="Directory to save frames/arrays")
        parser.add_argument("--fps", type=int, default=30, help="Target FPS to extract")
        parser.add_argument("--width", type=int, default=128, help="Frame width")
        parser.add_argument("--height", type=int, default=64, help="Frame height (must be multiple of 8)")
        parser.add_argument("--threshold", type=int, default=128, help="Binarization threshold (0-255)")
        parser.add_argument("--to-array", action="store_true", help="Convert frames to SSD1306 arrays")
        parser.add_argument("--preview", action="store_true", help="Preview binarized frames in a window")
        parser.add_argument("--invert", action="store_true", help="Invert colors (black ↔ white)")
        parser.add_argument("--compress", action="store_false", help="Compress arrays with RLE")
        return parser.parse_args()


if __name__ == "__main__":
    args = CommandHelper.create_options()
    CommandHelper.extract_frames(
        args.video_path,
        args.out_path,
        args.fps,
        args.width,
        args.height,
        args.threshold,
        args.to_array,
        args.preview,
        args.invert,
        args.compress,
    )
