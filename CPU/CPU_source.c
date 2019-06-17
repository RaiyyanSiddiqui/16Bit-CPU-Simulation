#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "CPU_header.h"

//#define MOD(a,b) (((a%b)+b)%b) //name conflict with opCode

#define REGISTER(a) process->registers[a]
#define FLAG(a)     process->flags[a]
#define P_OPERAND   process->operand

#define DIRECT(a)   memory->STORE[a]
#define INDIRECT(a) memory->STORE[memory->STORE[a]]

#define ASSIGN_CASE(OP,a,b,ret)    case OP:\
                                        a = b;\
                                        return ret

#define ASSIGN_IF_CASE(OP,boolE,a,b)    case OP:\
                                                if(boolE){\
                                                    a = b;\
                                                    return 1;\
                                                }\
                                                return 0
#define UNARY_CASE(OP,oper,a,ret)  case OP:\
                                         tempStorage = oper(a);\
                                         flagSet(process, tempStorage);\
                                         a = tempStorage;\
                                         return ret
// TEST NEGATIVE/OVERFLOW STUFF (unsigned subtraction)
#define BINARY_CASE(OP,oper,a,b,ret)  case OP:\
                                         tempStorage = a oper b;\
                                         flagSet(process, tempStorage);\
                                         a = tempStorage;\
                                         return ret

#define NBINARY_CASE(OP,oper,a,b,ret)  case OP:\
                                         tempStorage = ~(a oper b);\
                                         flagSet(process, tempStorage);\
                                         a = tempStorage;\
                                         return ret
// includes stdint.h, stdbool.h, string.h
/* IN HEADER
extern void initializeCPU(struct CPU *process);
extern void initializeRAM(struct RAM *memory);
extern void writeInstruction(struct RAM *, uint16_t, uint8_t, uint8_t);
extern void cleanupOperation(struct CPU *, struct RAM *);
extern uint8_t cpuStep(struct CPU *, struct RAM *);
extern void flagSet(struct CPU *, long);
extern void fetchInstruction(struct CPU *, struct RAM *);
extern void decodeInstruction(struct CPU *, struct RAM *);
extern int executeInstruction(struct CPU *, struct RAM *);
END IN HEADER */
//INLINE
inline void initializeCPU(struct CPU *process){
    process->clockEn = true;
    process->T = true; // true flag
    process->ON = 0xffff;
}
// INLINE
inline void initializeRAM(struct RAM *memory){
}
// INLINE
inline void cleanupOperation(struct CPU *process, struct RAM *memory){
    //printf("debug: resetting flags\n");
    memset(process->flags + 1, '\0',  (numFlags -1) * sizeof(bool)); // skips resetting the true flag
}
uint8_t cpuStep(struct CPU *process, struct RAM *memory){
    uint8_t cleanupFlag, curIntstruction;
    if(! process->clockEn){
        return 0;
    }
    //printf("\ndebug: getfetch\n");
    fetchInstruction(process, memory);
    //printf("debug: get assign flag\n");
    curIntstruction = process->opCode;
    //printf("debug: opCode: %x\n", curIntstruction);
    //printf("debug: decode instruction\n");
    decodeInstruction(process, memory);
    //printf("debug: execute instruction\n");
    cleanupFlag = executeInstruction(process, memory);
    //printf("debug: optional cleanup\n");
    if(cleanupFlag != 0){
        cleanupOperation(process, memory);
    }
    //printf("debug: check for memory overflow\n");
    if(process->PC == memSize -1){
        printf("FATAL ERROR, wrong use of opCode: %x, or memory limit reached!\n", curIntstruction);
        process->clockEn = false;
        return 0;
    }
    //printf("debug: increment PC\n");
    process->PC += 1;
    return 1;
}
void flagSet(struct CPU *process, long a){
    if(a > UINT16_MAX ){
        process->O = true;
        process->CA = true;
    }
    if(a > 0){
        process->PS = true;
    } else if(a < 0){
        process->N = true;
    } else{
        process->Z = true;
    }
}
// INLINE
// no staging for now, pipelining makes no sense here
inline void fetchInstruction(struct CPU *process, struct RAM *memory){
    //printf("debug: attempt to read addr. %x\n", process->PC);
    process->OP = memory->STORE[process->PC];
    //printf("debug: Got OP\ndebug: Decipher opCode\n");
    process->opCode   = (uint8_t)(process->OP >> 8);
    //printf("debug: Decipher operand\n");
    process->operand = (uint8_t)(process->OP & 0x00ff);
}
// INLINE  // no staging for now, pipelining makes no sense here
inline void decodeInstruction(struct CPU *process, struct RAM *memory){
}
int executeInstruction(struct CPU *process, struct RAM *memory){
    long tempStorage;
    switch(process->opCode){
        case SKP:
            return 0;
        ASSIGN_CASE(HLT, process->clockEn, false, 2);
        // Registers: ON (6) and ZE(7) are read-only
        ASSIGN_CASE(MOV, REGISTER(P_OPERAND >> 4), REGISTER(P_OPERAND & 0x0f),1);

        ASSIGN_CASE(LIA, REGISTER(0)                , P_OPERAND, 1);
        ASSIGN_CASE(LDA, REGISTER(0)                , DIRECT(P_OPERAND), 1);
        ASSIGN_CASE(LNA, REGISTER(0)                , INDIRECT(P_OPERAND), 1);
        ASSIGN_CASE(SRA, DIRECT(REGISTER(P_OPERAND)), REGISTER(0), 1);
        ASSIGN_CASE(SDA, DIRECT(P_OPERAND)          , REGISTER(0), 1);
        ASSIGN_CASE(SNA, INDIRECT(P_OPERAND)        , REGISTER(0), 1);

        ASSIGN_CASE(LIB, REGISTER(1)                , P_OPERAND, 1);
        ASSIGN_CASE(LDB, REGISTER(1)                , DIRECT(P_OPERAND), 1);
        ASSIGN_CASE(LNB, REGISTER(1)                , INDIRECT(P_OPERAND), 1);
        ASSIGN_CASE(SRB, DIRECT(REGISTER(P_OPERAND)), REGISTER(1), 1);
        ASSIGN_CASE(SDB, DIRECT(P_OPERAND)          , REGISTER(1), 1);
        ASSIGN_CASE(SNB, INDIRECT(P_OPERAND)        , REGISTER(1), 1);

        ASSIGN_CASE(LIC, REGISTER(2)                , P_OPERAND, 1);
        ASSIGN_CASE(LDC, REGISTER(2)                , DIRECT(P_OPERAND), 1);
        ASSIGN_CASE(LNC, REGISTER(2)                , INDIRECT(P_OPERAND), 1);
        ASSIGN_CASE(SRC, DIRECT(REGISTER(P_OPERAND)), REGISTER(2), 1);
        ASSIGN_CASE(SDC, DIRECT(P_OPERAND)          , REGISTER(2), 1);
        ASSIGN_CASE(SNC, INDIRECT(P_OPERAND)        , REGISTER(2), 1);

        ASSIGN_CASE(LID, REGISTER(3)                , P_OPERAND, 1);
        ASSIGN_CASE(LDD, REGISTER(3)                , DIRECT(P_OPERAND), 1);
        ASSIGN_CASE(LND, REGISTER(3)                , INDIRECT(P_OPERAND), 1);
        ASSIGN_CASE(SRD, DIRECT(REGISTER(P_OPERAND)), REGISTER(3), 1);
        ASSIGN_CASE(SDD, DIRECT(P_OPERAND)          , REGISTER(3), 1);
        ASSIGN_CASE(SND, INDIRECT(P_OPERAND)        , REGISTER(3), 1);

        ASSIGN_CASE(SRO, DIRECT(REGISTER(P_OPERAND)), REGISTER(6), 1);
        ASSIGN_CASE(SDO, DIRECT(P_OPERAND)          , REGISTER(6), 1);
        ASSIGN_CASE(SNO, INDIRECT(P_OPERAND)        , REGISTER(6), 1);

        ASSIGN_CASE(SRZ, DIRECT(REGISTER(P_OPERAND)), REGISTER(7), 1);
        ASSIGN_CASE(SDZ, DIRECT(P_OPERAND)          , REGISTER(7), 1);
        ASSIGN_CASE(SNZ, INDIRECT(P_OPERAND)        , REGISTER(7), 1);

        ASSIGN_CASE(   BRA         , REGISTER(4), REGISTER(4) + P_OPERAND - 1, 1);
        ASSIGN_IF_CASE(BZE, FLAG(1), REGISTER(4), REGISTER(4) + P_OPERAND - 1);
        ASSIGN_IF_CASE(BOV, FLAG(2), REGISTER(4), REGISTER(4) + P_OPERAND - 1);
        ASSIGN_IF_CASE(BCA, FLAG(3), REGISTER(4), REGISTER(4) + P_OPERAND - 1);
        ASSIGN_IF_CASE(BNG, FLAG(4), REGISTER(4), REGISTER(4) + P_OPERAND - 1);
        ASSIGN_IF_CASE(BPS, FLAG(5), REGISTER(4), REGISTER(4) + P_OPERAND - 1);
        ASSIGN_CASE(   BRR         , REGISTER(4), REGISTER(4) + REGISTER(P_OPERAND) - 1, 1);
        ASSIGN_CASE(   BRD         , REGISTER(4), REGISTER(4) + DIRECT(P_OPERAND)   - 1, 1);
        ASSIGN_CASE(   BRN         , REGISTER(4), REGISTER(4) + INDIRECT(P_OPERAND) - 1, 1);

        ASSIGN_CASE(   FAL         , REGISTER(4), REGISTER(4) - P_OPERAND - 1, 1);
        ASSIGN_IF_CASE(FZE, FLAG(1), REGISTER(4), REGISTER(4) - P_OPERAND - 1);
        ASSIGN_IF_CASE(FOV, FLAG(2), REGISTER(4), REGISTER(4) - P_OPERAND - 1);
        ASSIGN_IF_CASE(FCA, FLAG(3), REGISTER(4), REGISTER(4) - P_OPERAND - 1);
        ASSIGN_IF_CASE(FNG, FLAG(4), REGISTER(4), REGISTER(4) - P_OPERAND - 1);
        ASSIGN_IF_CASE(FPS, FLAG(5), REGISTER(4), REGISTER(4) - P_OPERAND - 1);
        ASSIGN_CASE(   FAR         , REGISTER(4), REGISTER(4) - REGISTER(P_OPERAND) - 1, 1);
        ASSIGN_CASE(   FAD         , REGISTER(4), REGISTER(4) - DIRECT(P_OPERAND)   - 1, 1);
        ASSIGN_CASE(   FAN         , REGISTER(4), REGISTER(4) - INDIRECT(P_OPERAND) - 1, 1);

        ASSIGN_CASE(   JMP         , REGISTER(4), P_OPERAND - 1, 1);
        ASSIGN_IF_CASE(JZE, FLAG(1), REGISTER(4), P_OPERAND - 1);
        ASSIGN_IF_CASE(JOV, FLAG(2), REGISTER(4), P_OPERAND - 1);
        ASSIGN_IF_CASE(JCA, FLAG(3), REGISTER(4), P_OPERAND - 1);
        ASSIGN_IF_CASE(JNG, FLAG(4), REGISTER(4), P_OPERAND - 1);
        ASSIGN_IF_CASE(JPS, FLAG(5), REGISTER(4), P_OPERAND - 1);
        ASSIGN_CASE(   JMR         , REGISTER(4), REGISTER(P_OPERAND) - 1, 1);
        ASSIGN_CASE(   JMD         , REGISTER(4), DIRECT(P_OPERAND)   - 1, 1);
        ASSIGN_CASE(   JMN         , REGISTER(4), INDIRECT(P_OPERAND) - 1, 1);

        BINARY_CASE(ADD, + , REGISTER(P_OPERAND >> 4), REGISTER(P_OPERAND & 0x0f), 0);
        BINARY_CASE(SUB, - , REGISTER(P_OPERAND >> 4), REGISTER(P_OPERAND & 0x0f), 0);
        BINARY_CASE(MUL, * , REGISTER(P_OPERAND >> 4), REGISTER(P_OPERAND & 0x0f), 0);
        BINARY_CASE(DIV, / , REGISTER(P_OPERAND >> 4), REGISTER(P_OPERAND & 0x0f), 0);
        BINARY_CASE(MOD, % , REGISTER(P_OPERAND >> 4), REGISTER(P_OPERAND & 0x0f), 0);
        // pow is not infix, it is instead forward polish notation
        //BINARY_CASE(POW,  , REGISTER(P_OPERAND >> 4), REGISTER(P_OPERAND & 0x0f), 0);
        UNARY_CASE(SIN,  sin, REGISTER(P_OPERAND), 0);
        UNARY_CASE(SNH, sinh, REGISTER(P_OPERAND), 0);
        UNARY_CASE(NEG,   - , REGISTER(P_OPERAND), 0);

        UNARY_CASE(NOT,  ~, REGISTER(P_OPERAND), 0);
        BINARY_CASE(AND, & , REGISTER(P_OPERAND >> 4), REGISTER(P_OPERAND & 0x0f), 0);
        BINARY_CASE(OR,  | , REGISTER(P_OPERAND >> 4), REGISTER(P_OPERAND & 0x0f), 0);
        BINARY_CASE(XOR, ^ , REGISTER(P_OPERAND >> 4), REGISTER(P_OPERAND & 0x0f), 0);
        NBINARY_CASE(NND,& , REGISTER(P_OPERAND >> 4), REGISTER(P_OPERAND & 0x0f), 0);
        NBINARY_CASE(NOR,| , REGISTER(P_OPERAND >> 4), REGISTER(P_OPERAND & 0x0f), 0);
        NBINARY_CASE(XNR,^ , REGISTER(P_OPERAND >> 4), REGISTER(P_OPERAND & 0x0f), 0);
        // should not assign to a VVV
        //BINARY_CASE(CMP, - , REGISTER(P_OPERAND >> 4), REGISTER(P_OPERAND & 0x0f), 0);
    }
    return 1;
}//*/
