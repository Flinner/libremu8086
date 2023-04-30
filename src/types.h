#pragma once

#include <cstdint>
#include <iostream>
#include <optional>
#include <tuple>
#include <variant>

using std::nullopt;

enum class R;

using u16 = uint16_t;
using u8 = uint8_t;
using i16 = int16_t;
using i8 = int8_t;
using i8ori16 = std::variant<i8, i16>;

using MEM =
    std::tuple<std::optional<R>, std::optional<R>, std::optional<i8ori16>>;

// clang-format off
enum class R {
  AX, AH, AL,
  BX, BH, BL,
  CX, CH, CL,
  DX, DH, DL,
  BP,
  SP,
  SI,
  DI,
  CS,
  DS,
  SS,
  ES,
  IP,
  // FX
};
// clang-format on

enum class OP {
  ADD,
  SUB,
  MUL,
};
