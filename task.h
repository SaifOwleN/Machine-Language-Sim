#ifndef MACHINE_LANGUAGE_SIMULATOR_H
#define MACHINE_LANGUAGE_SIMULATOR_H

#include <stdint.h>

// Define the opcodes for different instructions
#define OP_LOAD_MEM  0x1
#define OP_LOAD_VAL  0x2
#define OP_STORE_MEM 0x3
#define OP_STORE_SCR 0x3
#define OP_MOVE      0x4
#define OP_ADD_INT   0x5
#define OP_ADD_FLOAT 0x6
#define OP_JUMP      0xB
#define OP_HALT      0xC

// Define the registers and memory size
#define NUM_REGISTERS 16
#define MEMORY_SIZE   256

#endif // MACHINE_LANGUAGE_SIMULATOR_H
