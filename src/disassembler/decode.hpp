#pragma once
#include "../types.h"

// a queue is of size 6 bytes
extern u8 queue[6];

// Instruction Type
enum class IType {
  // 8 bit Register-Register
  R8R,
  // 16 bit Register-Register
  R16R,

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
  IType type;
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
