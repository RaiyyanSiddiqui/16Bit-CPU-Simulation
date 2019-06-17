#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "emulator_header.h"

// guaranteed to be initialized to 0
struct CPU cpu = {true, 0x00, 0x00,
                    .registers = {0x0000, 0x0000,
                    0x0000, 0x0000,
                    0x0000, 0x0000,
                    0xffff, 0x0000},
                    .flags = {true,  false, false,
                    false, false, false} };
struct RAM ram = {0};

// AVERAGE CPU performance: 17-25 MegaInstructions/s == ~30-40 MHz

struct DebugInfo{
    clock_t start, end;
    double cpu_time_used;
    uint32_t counter;

    uint16_t printLocationData;
} dbi = {0};

int main() {

    printf("Creating-> CPU: %d, RAM: %d Bytes\n", sizeof(struct CPU), sizeof(struct RAM));
    printf("Memory Size: %d\n", memSize);

    /* */// namespace program{
    // quadruple for loop
    writeInstruction(&ram, 0x0000, LID, 0x01); // write 1

    writeInstruction(&ram, 0x0001, LIA, 0xff); // write ff
    writeInstruction(&ram, 0x0002, SUB, 0x03); // SUB A D -> SUB 0 3
    writeInstruction(&ram, 0x0003, JZE, 0x0b); // Jump if ZEro -> 0x0B ->Break to program end

    writeInstruction(&ram, 0x0004, LIB, 0xff); // write ff
    writeInstruction(&ram, 0x0005, SUB, 0x13); // SUB B D -> SUB 1 3
    writeInstruction(&ram, 0x0006, JZE, 0x02); // Jump if ZEro ->0x02 ->Break to outer loop

    writeInstruction(&ram, 0x0007, LIC, 0xff); // write ff
    writeInstruction(&ram, 0x0008, SUB, 0x23); // SUB C D -> SUB 2 3
    writeInstruction(&ram, 0x0009, JZE, 0x05); // Jump if ZEro ->0x05 ->Break to outer loop

    writeInstruction(&ram, 0x000A, JMP, 0x08); // JMP 0x08-> inner loop

    writeInstruction(&ram, 0x000B, SDD, 0x10); // SDA 0x10 -> store A to memory 0x10
    writeInstruction(&ram, 0x000C, HLT, 0x00); // Halt
    dbi.printLocationData = 0x10;
    /* */// end namespace program

    printf("Starting Program\n");
    dbi.start = clock();
    while(cpuStep(&cpu, &ram)){
        dbi.counter++;
    }
    dbi.end = clock();

    printf("Program Finished\n");
    dbi.cpu_time_used = ((double)(dbi.end - dbi.start))/CLOCKS_PER_SEC;

    printf("\nTime -> Start: %lf, End:  %lf\n", (double)dbi.end, (double)dbi.start);
    printf("Time taken to execute: %lf seconds\n", dbi.cpu_time_used);
    printf("Physical Host CPU CLOCKS per second: %ld\n", (long)CLOCKS_PER_SEC);
    printf("\nNumber of Virtual CPU Instructions: %d\n", dbi.counter);
    if(dbi.cpu_time_used == 0){
        printf("Virtual CPU Instructions per second Inf.\n");
    } else{
        printf("Virtual CPU Instructions per second: %lf\n", dbi.counter / dbi.cpu_time_used);
    }
    // print desired memory location
    printf("%d\n", ram.STORE[dbi.printLocationData]);
    return 0;
}
