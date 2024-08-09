/* Pemu PDP-8 Emulator, written in C++, main source file.
 * Copyright (C) 2024 Jake Steinburger under the MIT license. See the GitHub repo's LICENSE file for more information.
 * This is my first emulator, so it may not be perfect, but I'm pretty happy with it so far despite it's limitations.
 *
 * Pemu is designed to work best with this PDP-8 assembler: https://github.com/Dcraftbg/Pdp8 (or as I call it, PASM)
 * It should theoretically work with other assemblers, but I haven't tested it with others.
 * At some point, I'll write a BASIC interpreter to run on this emulator so that you can try it out without writing assembly code.
 */

#include <stdint.h>
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

#include "include/utils.hpp"
#include "include/cpu.hpp"
#include "include/instructions.hpp"
#include "include/state.hpp"

struct LookupPair {
    Opcodes opc;
    void (*addr)(Instruction);
};

LookupPair lookupTable[] = {
    {AND, &runNotImplemented},
    {TAD, &runTAD},
    {ISZ, &runNotImplemented},
    {DCA, &runNotImplemented},
    {JMS, &runNotImplemented},
    {JMP, &runJMP},
    {LDI, &runLDI},
    {OPR, &runNotImplemented},
};

// where the state of the machine is stored
MemoryAddress memory[128][4096] = {0};
Registers registers = {0};

void runInstruction(Instruction instruction) {
    if (instruction.opc == LDI) instruction.addr = reverse3bit(instruction.addr);
    int idx = instruction.opc;
    (lookupTable[idx].addr)(instruction);
    printInstruction(instruction);
}
char* loadBin(char* fname, std::streamsize& size) {
    std::ifstream binFile(fname, std::ios::binary | std::ios::ate);
    if (!binFile.is_open()) {
        std::cout << "Invalid file path.\n";
        exit(1);
    }
    size = binFile.tellg();
    binFile.seekg(0, std::ios::beg);
    char* toReturn = new char[size];
    if (!binFile.read(toReturn, size)) {
        std::cout << "Can't read file.\n";
        delete[] toReturn;
        exit(1);
    }
    binFile.close();
    return toReturn;
}

void help() {
    std::cout << "Pemu PDP-8 Emulator version 0.0.1\nCopyright (C) 2024 Jake Steinburger under the MIT license. See the GitHub repo for more information.\nUsage details:\npemu [binary file to run]" << std::endl;
    exit(0);
}

int main(int argc, char** argv) {
    if (argc != 2) help();
    std::streamsize binFileSize = 0;
    char *program = loadBin(argv[1], binFileSize);
    int programLength = (binFileSize * 8) / 12;
    // load the instructions into memory at address 0x0
    MemoryAddress *programLoadAddr = (MemoryAddress*)(&memory);
    for (int i = 0; i < programLength; i++)
        programLoadAddr[i] = decodeU12(program, i, (int)binFileSize);
    while (1) {
        int page = (registers.PC >> 7) & 31;
        int address = registers.PC & 127;
        runInstruction(unpackInstruction(memory[page][address]));
        if (unpackInstruction(memory[page][address]).opc == JMP) continue;
        if ((registers.PC & 127) == 127)
            registers.PC = (((registers.PC >> 7) & 31) + 1) << 7;
        else
            registers.PC++;
    }
}
