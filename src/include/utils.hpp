/* header for ../utils.cpp
 * Copyright (c) 2024 Jake Steinburger under the MIT license. See the GitHub repo for more info.
 */

#include "cpu.hpp"
#include <stdint.h>

#ifndef UTILS_HPP
#define UTILS_HPP

void printInstruction(Instruction instr);

MemoryAddress packInstruction(Instruction instruction);

Instruction unpackInstruction(MemoryAddress instruction);

uint8_t reverse3bit(uint8_t original);

MemoryAddress decodeU12(char* data, int head, int end); 

#endif
