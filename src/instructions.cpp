/* Instruction implementations for PEmu.
 * Copyright (C) 2024 Jake Steinburger under the MIT license. See the GitHub repo for more information.
 */

#include "include/instructions.hpp"
#include "include/cpu.hpp"
#include "include/state.hpp"

#include <iostream>
#include <string>

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

