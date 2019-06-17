#ifndef __opcodesZ__
#define __opcodesZ__

// Immediate(I),Direct(D),Indirect(N)
enum Ops{ // referenced by 8bit opcode (255 vals)
    SKP,/*0x00*/      HLT,/*0x01*/      MOV,/*0x02*/

    LIA,/*0x03*/      LDA,/*0x04*/      LNA,/*0x05*/
    SRA,/*0x06*/      SDA,/*0x07*/      SNA,/*0x08*/  //SRA: looks at other register for address to store

    LIB,/*0x09*/      LDB,/*0x0A*/      LNB,/*0x0B*/
    SRB,/*0x0C*/      SDB,/*0x0D*/      SNB,/*0x0E*/  //SRB: looks at other register for address to store

    LIC,/*0x0F*/      LDC,/*0x10*/      LNC,/*0x11*/
    SRC,/*0x12*/      SDC,/*0x13*/      SNC,/*0x14*/  //SRC: looks at other register for address to store

    LID,/*0x15*/      LDD,/*0x16*/      LND,/*0x17*/
    SRD,/*0x18*/      SDD,/*0x19*/      SND,/*0x1A*/  //SRD: looks at other register for address to store
    ///////////////////////////////// Write Ones and Write Zeros
    //shifted by 3
    SRO,/*0x1B*/      SDO,/*0x1C*/      SNO,/*0x1D*/
    //shifted by 3
    SRZ,/*0x1E*/      SDZ,/*0x1F*/      SNZ,/*0x20*/
    //////////////////////////////////////////////
    BRA,/*0x21*/      BZE,/*0x22*/      BOV,/*0x23*/ // forward (branch)
    BCA,/*0x24*/      BNG,/*0x25*/      BPS,/*0x26*/
    BRR,/*0x27*/      BRD,/*0x28*/      BRN,/*0x29*/

    FAL,/*0x2A*/      FZE,/*0x2B*/      FOV,/*0x2C*/ // backward (fallback)
    FCA,/*0x2D*/      FNG,/*0x2E*/      FPS,/*0x2F*/
    FAR,/*0x30*/      FAD,/*0x31*/      FAN,/*0x32*/

    JMP,/*0x33*/      JZE,/*0x34*/      JOV,/*0x35*/ // absolute (jump)
    JCA,/*0x36*/      JNG,/*0x37*/      JPS,/*0x38*/
    JMR,/*0x39*/      JMD,/*0x3A*/      JMN,/*0x3B*/

    ADD,/*0x3C*/      SUB,/*0x3D*/      MUL,/*0x3E*/
    DIV,/*0x3F*/      MOD,/*0x40*/      POW,/*0x41*/
    SIN,/*0x42*/      SNH,/*0x43*/      NEG,/*0x44*/

    NOT,/*0x45*/
    AND,/*0x46*/       OR,/*0x47*/      XOR,/*0x48*/
    NND,/*0x49*/      NOR,/*0x4A*/      XNR,/*0x4B*/

    CMP,/*0x4C*/ // registers
    // compares 2 integers, if a is higher -> pos, elif a is lower -> neg, equal -> 0
};


#endif//__opcodesZ__
