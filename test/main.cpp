#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <fixed.hpp>
#include <fpmath.hpp>
#include <numbers>

int main()
{
    auto fp1 = fixed32(1);
    auto fp2 = fixed32(3);
    fp2 /= 114.514_f32;
    printf("%f, %f, %f\n", (float)fixed32::e(), (float)fixed32::pi(), (float)fixed32::double_pi());
    printf("%d, %f, %f\n", (int)fp1, (float)(114.514_f32 / 3), (float)fp2);
    printf("max = %f, min = %f\n", (float)f32_max, (float)f32_min);
    printf("abs = %f, eps = %f, compare = %b\n", (float)abs(-114.5_f32), (float)fixed32::nearly_compare_epsilon(), 1.114_f32 < 1.115_f32);
    printf("sqrt(114.514) = %f, %f\n", (float)sqrt(114.514_f32), std::sqrt(114.514));
    printf("sin(pi/6) = %f, %f\n", (float)sin(fixed32::pi() / 6), std::sin(std::numbers::pi / 6));
    printf("cos(pi/6) = %f, %f\n", (float)cos(fixed32::pi() / 6), std::cos(std::numbers::pi / 6));
    printf("tan(pi/6) = %f, %f\n", (float)tan(fixed32::pi() / 6), std::tan(std::numbers::pi / 6));
    printf("atan(1.114) = %f, %f\n", (float)atan(1.514_f32), std::atan(1.514));
    printf("cbrt(1.114) = %f, %f\n", (float)cbrt(-114.514_f32), std::cbrt(-114.514));
    printf("log2(10) = %f, %f\n", (float)log2(10_f32), std::log2(10));
    constexpr auto log2_10 = fixed32::template from_fixed_num_value<60>(0x35269E12F346E200ll);
    printf("ln(114.514) = %f, %f\n", (float)log(114.514_f32), std::log(114.514));
    printf("log10(114.514) = %f, %f\n", (float)log10(114.514_f32), std::log10(114.514));
    printf("exp(4) = %f, %f\n", (float)exp(4_f32), std::exp(4));
    printf("pow(1.14, 5.14) = %f, %f\n", (float)pow(1.14_f32, 5.14_f32), std::pow(1.14, 5.14));
}
