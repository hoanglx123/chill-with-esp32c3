#include <IBaseProgram.h>
// #include "DasaiMochi/DasaiMochi.h"
#include "Testing/Testing.h"

IBaseProgram* currentProgram = nullptr;

void setup() {
    currentProgram = new Testing();

    /* Initialize program */
    if (currentProgram) {
        currentProgram->initProgram();
    }
}

void loop() {
    /* Run program */
    if (currentProgram) {
        currentProgram->runProgram();
    }
}