#include <cmath>
#include <cstdlib>
#include <fixed.hpp>
#include <fpmath.hpp>
#include <numbers>
#include <fixed_formatter.hpp>
#include <papilio/format.hpp>
#include <papilio/print.hpp>

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
    printf("ceil(114.514) = %f, %f\n", (float)ceil(114.514_f32), std::ceil(114.514));
    printf("ceil(32767.5) = %f, %f\n", (float)ceil(32767.5_f32), std::ceil(32767.5));
    printf("floor(-114.514) = %f, %f\n", (float)floor(-114.514_f32), std::floor(-114.514));
    printf("floor(-32767.5) = %f, %f\n", (float)floor(-32767.5_f32), std::floor(-32767.5));
    printf("round(-114.514) = %f, %f\n", (float)round(-114.514_f32), std::round(-114.514));
    printf("round(-114.414) = %f, %f\n", (float)round(-114.414_f32), std::round(-114.414));
    printf("round(-32767.5) = %f, %f\n", (float)round(-32767.5_f32), std::round(-32767.5));
    printf("round(-32767.4) = %f, %f\n", (float)round(-32767.4_f32), std::round(-32767.4));
    printf("round(114.514) = %f, %f\n", (float)round(114.514_f32), std::round(114.514));
    printf("round(114.414) = %f, %f\n", (float)round(114.414_f32), std::round(114.414));
    printf("round(32767.5) = %f, %f\n", (float)round(32767.5_f32), std::round(32767.5));
    printf("round(32767.4) = %f, %f\n", (float)round(32767.4_f32), std::round(32767.4));
    auto modf_b = 1.15;
    printf("5.14 mod 1.14 = %f, %f\n", (float)(5.14_f32 % 1.14_f32), std::fmod(5.14, 1.14));
    auto fp = fixed32(0);
    if(f32_from_cstring("-114.514", 8, fp))
        printf("test from_cstring: %f, %f\n", (float) fp, (float) -114.514_f32);
    fp = 0_f32;
    printf("test parse: %s, %f, %f\n", parse("114b.514a", "a", fp),(float) fp, (float) 114.514_f32);
    printf("test parse: %s, %f, %f\n", parse("114.514a", "a", fp),(float) fp, (float) 114.514_f32);
    printf("test parse: %s, %f, %f\n", parse("114a.514a", "a", fp),(float) fp, (float) 114.514_f32);
    // fp = 0_f32;
    // std::cin >> fp;
    // printf("test cin: %f(%d)\n", (float) fp, fp.inner_value());
    std::cout << 114.5625_f32 << std::endl;
    std::cout << -114.5625_f32 << std::endl;

    papilio::println("papilio format: {}", 114.5625_f32);
}
