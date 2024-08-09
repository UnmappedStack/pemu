/* This is a header which defines CPU structures.
 * Copyright (C) 2024 Jake Steinburger under the MIT license. See the GitHub repo for more information.
 */

#include <stdint.h>

#ifndef CPU_HPP
#define CPU_HPP

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

#endif
