#include "decode.hpp"
#include "../types.h"
#include <iostream>

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
