/* Utility functions for PEmu
 * Copyright (c) 2024 Jake Steinburger under the MIT license. See the github repo for more info.
 */

#include "include/utils.hpp"
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>


void printInstruction(Instruction instr) {
    std::cout << instr.opc << " : " << instr.I << " : " << instr.Z << " : " << std::to_string(instr.addr) << std::endl;
}

MemoryAddress packInstruction(Instruction instruction) {
    MemoryAddress toReturn;
    toReturn.word = (instruction.opc & 0b111) | ((instruction.I & 1) << 3) | ((instruction.Z & 1) << 4) | ((instruction.addr & 0b1111111) << 5);
    return toReturn;
}

Instruction unpackInstruction(MemoryAddress instruction) {
    Instruction toReturn;
    toReturn.opc = (Opcodes)(instruction.word & 0b111);
    toReturn.I = instruction.word >> 3;
    toReturn.Z = instruction.word >> 4;
    toReturn.addr = instruction.word >> 5;
    return toReturn;
}

uint8_t reverse3bit(uint8_t original) {
    uint8_t toReturn;
    toReturn |= (0b100000 & original) >> 4;
    toReturn |= (0b001000 & original) >> 4;
    toReturn |= (0b010000 & original) >> 4;
    return toReturn;
}

MemoryAddress decodeU12(char* data, int head, int end) {
    char *byteAddr = (char*)((intptr_t)data + head * 12 / 8);
    MemoryAddress toReturn;
    if (head % 2 == 0) 
        toReturn.word = ((uint16_t)(byteAddr[0])     ) | ((((uint16_t)byteAddr[1]) & 0b1111) << 8);
    else
        toReturn.word = ((uint16_t)(byteAddr[0] >> 4)) | (byteAddr[1]                        << 4);
    return toReturn;
}
