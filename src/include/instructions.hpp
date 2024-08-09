/* header for ../instructions.cpp
 * Copyright (C) 2024 Jake Steinburger under the MIT license. See the GitHub repo for more info.
 */

#ifndef INSTRUCTIONS_HPP
#define INSTRUCTIONS_HPP

#include "cpu.hpp"

void runJMP(Instruction instruction);

void runTAD(Instruction instruction);

void runLDI(Instruction instruction);

void runNotImplemented(Instruction instruction); 

#endif
