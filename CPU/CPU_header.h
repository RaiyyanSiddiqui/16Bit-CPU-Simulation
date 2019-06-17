#ifndef __CPU_TEST__
#define __CPU_TEST__

#include <stdint.h>
#include <stdbool.h>
#include "op_codes_header.h" // enum Ops -> opcodes

#define memSize 1024000 // 64000 actual, 800 debug
#define numRegisters 8
#define numFlags 6
/*
    LAYOUT:
        Register Bus: All regs
        IO BUS:

    PC of mSize-1 is ERROR, (memory overflow, or error in execution)

    A B C D PC OP ON ZE -> 0 1 2 3 4 5 6 7, where 6 and 7 are read only
*/

// static/extern 'inline' function
// extern means that its defined in ANOTHER file, static means its only inside current file
// static in header is WEIRD. (here used so it doesnt complain about being accessed before being declared in file in question)

struct CPU{
    bool clockEn;
    uint8_t opCode, operand; // placeholder for split OP

    union{ // PC = mSize-1 is ERROR
        // A,B,C,D Program counter, ZERO register, 1s register
        uint16_t registers[numRegisters];
        struct{
            uint16_t A, B, C, D, PC, OP, ON, ZE;
            //16 bit so it can operate on PC or ram block directly, OP is current operation
        };
    }; // always true, zero, overflow, carry, negativem positive
    union{
        bool flags[numFlags];
        struct{
            bool T, Z, O, CA, N, PS; // T is always true
        };
    };
};

struct RAM{
    uint16_t STORE[memSize]; // 64 kbytes
};

extern void initializeCPU(struct CPU *process);
extern void initializeRAM(struct RAM *memory);

extern void cleanupOperation(struct CPU *, struct RAM *);

extern uint8_t cpuStep(struct CPU *, struct RAM *);

extern void flagSet(struct CPU *, long);

extern void fetchInstruction(struct CPU *, struct RAM *);
extern void decodeInstruction(struct CPU *, struct RAM *);
extern int executeInstruction(struct CPU *, struct RAM *); // returns whether cleanup should be execited

#endif/*__CPU_TEST__*/
