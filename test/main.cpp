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
    ++fp2;
    printf("%f, %f, %b\n", (float)abs(-114.5_f32), (float)fixed32::nearly_compare_epsilon(), 1.114_f32 < 1.115_f32);
    printf("%f, %f, %f\n", (float)sqrt(1 / 400_f32), (float)sin(fixed32::pi() / 6), (float)cos(fixed32::pi() / 3));
    printf("%f, %f, %f\n", (float)tan(fixed32::pi() / 4), (float)atan(1_f32), (float)cbrt(-114.514_f32));
    constexpr auto log2_e = fixed32::template from_fixed_num_value<30>(0x5C551D80ll);
    printf("fixed32 max: %f, ln = %f, %f\n", (float)f32_max, (float)log2(fixed32::e()), (float) log2_e);
}
