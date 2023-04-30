#include "decode.hpp"
#include "../types.h"
#include <iostream>

/**
 * @brief decodes the field present in the second byte (`R/M`)
 *        It also takes into account the third and fourth bytes
 *        that may or may not be used as displacments
 */
REG_M decode_R_M(u8 MOD, u8 R_M, bool W, i8 disp_lower, i8 disp_upper) {
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

Instruction decode(u8 queue[6]) {
  auto q = queue;
  auto b1 = q[0], b2 = q[1], b3 = q[2];
  auto b4 = q[3], b5 = q[4], b6 = q[5];

  Instruction inst;

  // Variables ending in _ mean they are "raw"

  // clang-format off
  // Raw decoding from bytes
  u8 opcode_      = (b1 & OPCODE_MASK   )     ;
  bool direction_ = (b1 & DIRECTION_MASK) >> 1;
  bool width_     = (b1 & WIDTH_MASK    )     ;
  u8 mod_         = (b1 & MOD_MASK      ) >> 6;
  u8 reg_         = (b1 & REG_MASK      ) >> 3;
  u8 reg_m_       = (b1 & REG_M_MASK    )     ;

  // Decode into C++ types
  R reg       = REG_MAP[width_][reg_];
  REG_M reg_m = decode_R_M(mod_, reg_m_, width_, b3, b4);
  // clang-format on

  // Using the values

  inst.reg = reg;
  inst.r_m = reg_m;
  inst.direction = direction_;
  switch (opcode_) {
    // ADD
  case 0x00:
  case 0x04:
    inst.op = OP::ADD;
  case 0x20:
  case 0x2C:
    inst.op = OP::SUB;
    break;
  default:
    std::cout << "TODO: OP Not implemented" << std::endl;
    exit(2);
  };
  return inst;
}
