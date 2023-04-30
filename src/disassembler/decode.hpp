#pragma once
#include "../types.h"

// clang-format off
#define OPCODE_MASK    0xFC   // 0b1111'1100
#define DIRECTION_MASK 0x02   // 0b0000'0010
#define WIDTH_MASK     0x01   // 0b0000'0001
#define MOD_MASK       0xC0   // 0b1100'0000
#define REG_MASK       0x38   // 0b0011'1000
#define REG_M_MASK     0x07   // 0b0000'0111
// clang-format on

/**
 * FORMAT: [W][REG]
 *  - W       : Width bit, 0 = 8 bits, 1 = 16
 *  - REG     : REG code
 */
const R REG_MAP[2][8] = {
    // W = 0
    {
        R::AL, // REG = 000  // 0
        R::CL, // REG = 001  // 1
        R::DL, // REG = 010  // 2
        R::BL, // REG = 011  // 3
        R::AH, // REG = 100  // 4
        R::CH, // REG = 101  // 5
        R::DH, // REG = 110  // 6
        R::BH, // REG = 111  // 7
    },
    // W = 1
    {
        R::AX, // REG =  000  // 0
        R::CX, // REG =  001  // 1
        R::DX, // REG =  010  // 2
        R::BX, // REG =  011  // 3
        R::SP, // REG =  100  // 4
        R::BP, // REG =  101  // 5
        R::SI, // REG =  110  // 6
        R::DI, // REG =  111  // 7
    }};

using MEM =
    std::tuple<std::optional<R>, std::optional<R>, std::optional<i8ori16>>;

/**
 * FORMAT: [MOD][REG_M]
 */

const MEM M_MAP[4][8] = {

    // MOD = 00
    // Memory operand with no displacment
    {
        {R::BX, R::SI, nullopt},    // 000
        {R::BX, R::DI, nullopt},    // 001
        {R::BP, R::SI, nullopt},    // 010
        {R::BP, R::DI, nullopt},    // 011
        {nullopt, R::SI, nullopt},  // 100
        {nullopt, R::DI, nullopt},  // 101
        {nullopt, nullopt, (i16)0}, // 110
        {R::BX, nullopt, nullopt},  // 111
    },

    // MOD = 01
    // 8 bit displacement
    {
        {R::BX, R::SI, (i8)0},   // 000
        {R::BX, R::DI, (i8)0},   // 001
        {R::BP, R::SI, (i8)0},   // 010
        {R::BP, R::DI, (i8)0},   // 011
        {nullopt, R::SI, (i8)0}, // 100
        {nullopt, R::DI, (i8)0}, // 101
        {nullopt, R::BP, (i8)0}, // 110
        {R::BX, nullopt, (i8)0}, // 111
    },

    // MOD = 10
    // 16 bit displacement
    {
        {R::BX, R::SI, (i16)0},   // 000
        {R::BX, R::DI, (i16)0},   // 001
        {R::BP, R::SI, (i16)0},   // 010
        {R::BP, R::DI, (i16)0},   // 011
        {nullopt, R::SI, (i16)0}, // 100
        {nullopt, R::DI, (i16)0}, // 101
        {nullopt, R::BP, (i16)0}, // 110
        {R::BX, nullopt, (i16)0}, // 111
    },

    // MOD = 11
    {},
};

/**
   @brief This represents an a decoded instruction

   # Instruction Types

   #### One-byte
   These only use:
   - `op`

   #### Register to Register
   These use:
   - `op`
   - `r1`
   - `r2`

 */
class Instruction {
public:
  Instruction() {}
  virtual ~Instruction() {}
  virtual std::string print() const { return ""; };

  OP op;
  union {
    R reg_desintaion;
    u8 reg_memory;
  };
  union {
    R reg_source;
    u8 mem_source;
  };
};

Instruction decode(u8 b1, u8 b2 = 0, u8 b3 = 0, u8 b4 = 0, u8 b5 = 0,
                   u8 b6 = 0);

Instruction decode(u8 queue[6]);

std::variant<R, MEM> decode_R_M(u8 MOD, u8 R_M, bool W, i8 disp_lower,
                                i8 disp_upper);
