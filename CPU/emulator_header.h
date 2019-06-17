#ifndef __cpu_emulation__
#define __cpu_emulation__

#include "CPU_header.h"

extern void writeInstruction(struct RAM *, uint16_t, uint8_t, uint8_t);
extern void parseFile(char *, struct RAM *);

#endif//__cpu_emulation__
