#include "decode.hpp"
#include "../types.h"
#include <iostream>
#include <optional>
#include <tuple>
#include <variant>

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
R REG_MAP[2][8] = {
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

/**
 * FORMAT: [MOD][REG_M]
 */

MEM M_MAP[4][8] = {

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

// Decodes the R/M field.
std::variant<R, MEM> decode_R_M(u8 MOD, u8 R_M, bool W, i8 disp_lower,
                                i8 disp_upper) {
  // Decode if it is a register first.
  if (MOD == 0b11) {
    return REG_MAP[W][R_M];

  } else { // it is a Memory address
    auto m = M_MAP[MOD][R_M];

    // sometimes there
    auto &maybe_disp = std::get<2>(m);

    if (maybe_disp.has_value()) {
      auto &disp = maybe_disp.value();

      if (i8 *disp8 = std::get_if<i8>(&disp)) {
        *disp8 = disp_lower;
      } else if (i16 *disp16 = std::get_if<i16>(&disp)) {
        *disp16 = (disp_upper << 8) + disp_lower;
      } else {
        std::cout << "How did this happen?" << std::endl;
        exit(1);
      };
      //
    }
    return m;
  }
}

void ADD(Instruction &inst, bool D, bool W, u8 REG, u8 MOD, u8 REG_M) {
  inst.op = OP::ADD;
}

void source_desintation(bool D, bool W, u8 REG, u8 MOD, u8 REG_M) {
  R _REG = REG_MAP[W][REG];
}

Instruction decode(u8 queue[6]) {
  auto q = queue;
  auto b1 = q[0], b2 = q[1], b3 = q[2];
  auto b4 = q[3], b5 = q[4], b6 = q[5];

  Instruction inst;

  // The opcode only
  u8 opcode = b1 & OPCODE_MASK;

  //  D = 1 indicates data is transfered TO the register
  bool direction = (b1 & DIRECTION_MASK) >> 1;

  // W = 0 for 8 BIT. W = 1 for 16 bit
  bool width = (b1 & WIDTH_MASK);

  // :)
  u8 MOD = (b1 & MOD_MASK) >> 6;

  u8 REG = (b1 & REG_MASK) >> 3;
  u8 REG_M = (b1 & REG_M_MASK);

  switch (opcode) {
  case 0x00:
    ADD(inst, direction, width, REG, MOD, REG_M);
    break;
  default:
    std::cout << "TODO: OP Not implemented" << std::endl;
    exit(2);
  };
  return inst;
}
