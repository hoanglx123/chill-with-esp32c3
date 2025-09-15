#!/usr/bin/env python3
import cv2
import os
import argparse
import sys
import shutil
import numpy as np

from pathlib import Path

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
    def save_array(out_path, name, arr, width, height):
        c_file = Path(out_path) / "frames_auto_generated.h"
        mode = "a" if c_file.exists() else "w"
        with open(c_file, mode) as f:
            if mode == "w":
                f.write("// Auto-generated bitmaps for SSD1306\n\n")
                f.write("#include <vector>\n\n")
            f.write(f"// Frame: {name} ({width}x{height})\n")
            f.write(f"static const std::vector<uint8_t> {name} = {{\n")
            for i, val in enumerate(arr):
                if i % 16 == 0:
                    f.write("    ")
                f.write(f"0x{val:02X}, ")
                if (i + 1) % 16 == 0:
                    f.write("\n")
            f.write("};\n\n")

    @staticmethod
    def save_index(out_path, frame_names):
        c_file = Path(out_path) / "frames_auto_generated.h"
        with open(c_file, "a") as f:
            f.write("// List of all frame arrays\n")
            f.write("static const std::vector<std::vector<uint8_t>> all_frames = {\n")
            for name in frame_names:
                f.write(f"    {name},\n")
            f.write("};\n")

    # ------------------------------
    # Main extractor
    # ------------------------------
    @staticmethod
    def extract_frames(video_path, out_path, fps, width, height, threshold, to_array, preview, invert):
        cap = cv2.VideoCapture(video_path)
        if not cap.isOpened():
            print(f"❌ Error: Cannot open video {video_path}")
            return

        # Clean output folder
        if os.path.exists(out_path):
            shutil.rmtree(out_path)
        os.makedirs(out_path, exist_ok=True)

        video_fps = cap.get(cv2.CAP_PROP_FPS) or fps
        frame_interval = max(int(video_fps // fps), 1)

        frame_count, saved_count = 0, 0
        frame_names = []

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
                    arr = CommandHelper.frame_to_array(binary)
                    array_name = f"frame_{saved_count:05d}_bitmap"
                    CommandHelper.save_array(out_path, array_name, arr, width, height)
                    frame_names.append(array_name)

                saved_count += 1

            frame_count += 1

        cap.release()
        if preview:
            cv2.destroyAllWindows()

        if to_array and frame_names:
            CommandHelper.save_index(out_path, frame_names)

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
    )
