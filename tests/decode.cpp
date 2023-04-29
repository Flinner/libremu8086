#include "../src/disassembler/decode.hpp"

#include <gtest/gtest.h>

void decodeRM_test(u8 MOD, u8 R_M, bool W, i8 disp_lower, i8 disp_upper, //
                   std::optional<R> first, std::optional<R> second,
                   std::optional<i8ori16> disp) {

  // std::cout << "MOD: " << MOD + 0 << std::endl;
  // std::cout << "R_M: " << R_M + 0 << std::endl;
  // std::cout << " == " << std::endl;
  auto m = std::get<MEM>(decode_R_M(MOD, R_M, W, disp_lower, disp_upper));

  auto _first = std::get<0>(m);
  auto _second = std::get<1>(m);
  auto _disp = std::get<2>(m);

  EXPECT_EQ(_first, first);
  EXPECT_EQ(_second, second);
  EXPECT_EQ(_disp, disp);
}

TEST(DecodeRM, NoDisp000) {
  decodeRM_test(0b00, 0b000, 1, 8, 8, //
                R::BX, R::SI, nullopt);
}

TEST(DecodeRM, NoDisp001) {
  decodeRM_test(0b00, 0b001, 1, 8, 8, //
                R::BX, R::DI, nullopt);
}
TEST(DecodeRM, NoDisp010) {
  decodeRM_test(0b00, 0b010, 1, 8, 8, //
                R::BP, R::SI, nullopt);
}
TEST(DecodeRM, NoDisp011) {
  decodeRM_test(0b00, 0b011, 1, 8, 8, //
                R::BP, R::DI, nullopt);
}
TEST(DecodeRM, NoDisp100) {
  decodeRM_test(0b00, 0b100, 1, 8, 8, //
                nullopt, R::SI, nullopt);
}
TEST(DecodeRM, NoDisp101) {
  decodeRM_test(0b00, 0b101, 1, 8, 8, //
                nullopt, R::DI, nullopt);
}
TEST(DecodeRM, NoDisp110) {
  decodeRM_test(0b00, 0b110, 1, 2, 2, //
                nullopt, nullopt, (i16)0x0202);
}
TEST(DecodeRM, NoDisp111) {
  decodeRM_test(0b00, 0b111, 1, 8, 8, //
                R::BX, nullopt, nullopt);
}
