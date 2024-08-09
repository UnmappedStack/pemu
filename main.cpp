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

// define the structures of the CPU
enum Opcodes {
    AND = 0b000,
    TAD = 0b001,
    ISZ = 0b010,
    DCA = 0b011,
    JMS = 0b100,
    JMP = 0b101,
    LDI = 0b110, // think this should be IOT? uhhhhh no not rn. Read the GitHub readme for info about why tf I'm doing it like this.
    OPR = 0b111
};

struct Instruction {
    Opcodes opc;
    bool I;
    bool Z;
    uint8_t addr;
};

struct Registers {
    int PC; // program counter
    int DF; // data field
    int AC; // accumulator
    int MQ; // multiplier quotient
};

struct MemoryAddress {
    uint16_t word : 12;
}__attribute__((packed));

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

struct LookupPair {
    Opcodes opc;
    void (*addr)(Instruction);
};

// where the state of the machine is stored
MemoryAddress memory[128][4096] = {0};
Registers registers = {0};

uint8_t reverse3bit(uint8_t original) {
    uint8_t toReturn;
    toReturn |= (0b100000 & original) >> 4;
    toReturn |= (0b001000 & original) >> 4;
    toReturn |= (0b010000 & original) >> 4;
    return toReturn;
}

// and the stuff that does stuff
void printInstruction(Instruction instr) {
    std::cout << instr.opc << " : " << instr.I << " : " << instr.Z << " : " << std::to_string(instr.addr) << std::endl;
}

void runJMP(Instruction instruction) {
    int prevPage = registers.PC >> 7;
    registers.PC = instruction.addr;
    if (!instruction.Z) registers.PC |= prevPage;
    std::cout << "Jumping to memory address " << std::to_string(registers.PC & 127) << std::endl;
}

void runTAD(Instruction instruction) {
    int memPage = registers.PC >> 7;
    int memAddr = instruction.addr;
    if (instruction.Z) memPage = 0;
    registers.AC += memory[memPage][memAddr].word;
    std::cout << "Added " << std::to_string(memory[memPage][memAddr].word) << " to AC, now equal to " << std::to_string(registers.AC) << std::endl;
}

void runLDI(Instruction instruction) {
    memory[0][126].word = instruction.addr;
    std::cout << "Loaded immediate value " << std::to_string(memory[0][126].word) << " into memory address: page 0, address 126.\n";
}

void runNotImplemented(Instruction instruction) {
    std::cout << "Instruction opcode not implemented yet.\n";
}

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

void runInstruction(Instruction instruction) {
    if (instruction.opc == LDI) instruction.addr = reverse3bit(instruction.addr);
    int idx = instruction.opc;
    (lookupTable[idx].addr)(instruction);
    printInstruction(instruction);
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
            registers.PC = (((registers.PC >> 7) && 31) + 1) << 7;
        else
            registers.PC++;
    }
}
