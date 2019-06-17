#include <stdio.h>
#include <stdlib.h>

#include "emulator_header.h"

// INLINE
inline void writeInstruction(struct RAM *memory, uint16_t address, uint8_t opCode, uint8_t operand){
    memory->STORE[address] = ((uint16_t)(opCode << 8)) + ((uint16_t)(operand));
}

void parseFile(char *filepath, struct RAM *memory){
    FILE *file;
    if((file = fopen(filepath, "r"))== NULL){
        puts("ERR, could not locate file");
        exit(EXIT_FAILURE);
    }



    fclose(file);
}
