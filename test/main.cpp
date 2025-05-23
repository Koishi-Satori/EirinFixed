#include <cmath>
#include <eirin/eirin.hpp>
#include <numbers>
#include <papilio/print.hpp>
#include <eirin/ext/papilio_integration.hpp>
#include <gtest/gtest.h>
// #include <eirin/detail/util.hpp>

using namespace eirin;

TEST(fixed_num, construct)
{
    auto fp1 = 0_f32;
    EXPECT_EQ((int)fp1, 0);
    parse("-114.514a", "a", fp1);
    EXPECT_EQ(fp1, -114.514_f32);

#ifdef EIRIN_FIXED_HAS_INT128
    auto fp2 = 0_f64;
    EXPECT_EQ((int)fp2, 0);
    EXPECT_EQ("-114.514"_f64, -114.514_f64);
#endif
}

TEST(fixed32, operator)
{
    auto fp1 = 0_f32;
    EXPECT_EQ(++fp1, 1_f32);
    fp1 = 1.14_f32;
    EXPECT_EQ(fp1 + 5.14_f32, 6.28_f32);
    EXPECT_EQ(fp1 * 2, 2.28_f32);
    EXPECT_EQ(514_f32 / 2, 257_f32);
    EXPECT_EQ(515_f32 % 2_f32, 1_f32);
    EXPECT_EQ(--fp1, 0.14_f32);

    EXPECT_THROW(
        (void)(1_f32 / 0),
        divide_by_zero
    );
}

TEST(fixed32, rounding)
{
    EXPECT_EQ(round(114.414_f32), 114_f32);
    EXPECT_EQ(round(114.514_f32), 115_f32);
    EXPECT_EQ(round(-114.414_f32), -114_f32);
    EXPECT_EQ(round(-114.514_f32), -115_f32);
}

TEST(fixed32, decompression)
{
    EXPECT_FALSE(signbit(0_f32));
    EXPECT_FALSE(signbit(1_f32));
    EXPECT_TRUE(signbit(-1_f32));

    EXPECT_EQ((1_f32).raw_integral_part(), 1);
    EXPECT_EQ((-1_f32).raw_integral_part(), 32767);

    EXPECT_EQ((1_f32).integral_part(), 1);
    EXPECT_EQ((-1_f32).integral_part(), 1);

    EXPECT_EQ((1_f32).fractional_part(), 0);
    EXPECT_EQ((-1_f32).fractional_part(), 0);

    {
        auto val = 1_f32 / 2;
        EXPECT_EQ(val.integral_part(), 0);
        EXPECT_EQ(val.fractional_part(), 1ULL << (val.precision - 1));
    }
}

namespace test_math
{
static testing::AssertionResult expect_fixed_eq(
    const eirin::fixed32& lhs,
    const eirin::fixed32& rhs,
    const eirin::fixed32& eps = fixed32::nearly_compare_epsilon()
)
{
    auto diff = abs(lhs - rhs);
    if(diff > eps)
        return testing::AssertionFailure()
               << lhs << " != " << rhs << ", diff: " << diff
               << ", Internal values: " << lhs.internal_value() << ", " << rhs.internal_value();
    else
        return testing::AssertionSuccess();
}

#ifdef EIRIN_FIXED_HAS_INT128
static testing::AssertionResult expect_fixed_eq(
    const eirin::fixed64& lhs,
    const eirin::fixed64& rhs,
    const eirin::fixed64& eps = fixed64::nearly_compare_epsilon()
)
{
    auto diff = abs(lhs - rhs);
    if(diff > eps)
        return testing::AssertionFailure()
               << lhs << " != " << rhs << ", diff: " << diff
               << ", Internal values: " << lhs.internal_value() << ", " << rhs.internal_value();
    else
        return testing::AssertionSuccess();
}
#endif
} // namespace test_math

TEST(fixed32, math)
{
    using test_math::expect_fixed_eq;

    EXPECT_EQ(abs(-114.514_f32), 114.514_f32);
    EXPECT_EQ(sin(0_f32), 0_f32);
    EXPECT_TRUE(expect_fixed_eq(sin(1_f32), 0.841471_f32));
    EXPECT_TRUE(expect_fixed_eq(sin(fixed32::pi() / 6), 0.5_f32));
    EXPECT_EQ(cos(0_f32), 1_f32);
    EXPECT_TRUE(expect_fixed_eq(cos(fixed32::pi() / 3), 0.5_f32));
    EXPECT_TRUE(expect_fixed_eq(cos(1_f32), 0.540302_f32));
    EXPECT_EQ(tan(0_f32), 0_f32);
    EXPECT_TRUE(expect_fixed_eq(tan(1_f32), 1.557407_f32));
    EXPECT_EQ(atan(0_f32), 0_f32);
    EXPECT_EQ(atan(1_f32), 0.785398_f32);
    EXPECT_EQ(sqrt(0_f32), 0_f32);
    EXPECT_EQ(sqrt(4_f32), 2_f32);
    EXPECT_EQ(sqrt(114.514_f32), 10.701121_f32);
    EXPECT_TRUE(expect_fixed_eq(cbrt(0_f32), 0_f32));
    EXPECT_EQ(cbrt(1_f32), 1_f32);
    EXPECT_EQ(cbrt(8_f32), 2_f32);
    EXPECT_EQ(cbrt(27_f32), 3_f32);
    EXPECT_EQ(log2(2_f32), 1_f32);
    EXPECT_EQ(log2(10_f32), 3.321928_f32);
    EXPECT_TRUE(expect_fixed_eq(log(fixed32::e()), 1_f32));
    EXPECT_TRUE(expect_fixed_eq(log(114.514_f32), 4.740697_f32));
    EXPECT_TRUE(expect_fixed_eq(log10(10_f32), 1_f32));
    EXPECT_TRUE(expect_fixed_eq(log10(114.514_f32), 2.058859_f32));
    EXPECT_TRUE(expect_fixed_eq(exp(1_f32), fixed32::e()));
    EXPECT_TRUE(expect_fixed_eq(radians(180_f32), numbers::pi));
    EXPECT_TRUE(expect_fixed_eq(degrees(numbers::pi), 180_f32));
}

TEST(fixed32, papilio_format)
{
    EXPECT_EQ(papilio::format("{:?}", 0_f32), "0");
    EXPECT_EQ(papilio::format("{:#?}", -1_f32), "-0x10000");

    EXPECT_EQ(papilio::format("{:d}", 0_f32), "0");
    EXPECT_EQ(papilio::format("{:d}", -1_f32), "-1");
    EXPECT_EQ(papilio::format("{0:-d},{0:+d},{0: d}", 0_f32), "0,+0, 0");
    EXPECT_EQ(papilio::format("{0:-d},{0:+d},{0: d}", -1_f32), "-1,-1,-1");

    EXPECT_EQ(papilio::format("{}", 0_f32), "0");
    EXPECT_EQ(papilio::format("{}", 1_f32), "1");

    EXPECT_EQ(papilio::format("{:#04x}", 15_f32), "0x0f");
    EXPECT_EQ(papilio::format("{:#04X}", 15_f32), "0X0F");

    {
        auto val = 1_f32 / 2;
        EXPECT_EQ(papilio::format("{}", val), "0.5");
        EXPECT_EQ(papilio::format("{:g}", val), "0.5");
        EXPECT_EQ(papilio::format("{:f}", val), "0.500000");
        EXPECT_EQ(papilio::format("{:.2f}", val), "0.50");
    }

    EXPECT_EQ(papilio::format("{:d}", fixed32::pi()), "3");
    EXPECT_EQ(papilio::format("{:.2g}", fixed32::pi()), "3.14");
    EXPECT_EQ(papilio::format("{:.2f}", fixed32::pi()), "3.14");
    EXPECT_EQ(papilio::format("{:.4f}", fixed32::pi()), "3.1415");

    EXPECT_THROW(
        (void)papilio::format("{:s}", fixed32::pi()),
        papilio::format_error
    );
}

TEST(fixed_num, constants)
{
    GTEST_LOG_(INFO) << "fixed32 max value: " << max_value<fixed32>() << ", min value: " << min_value<fixed32>();
    GTEST_LOG_(INFO) << "fixed32 e value: " << fixed32::e() << ", pi value: " << fixed32::pi();
    GTEST_LOG_(INFO) << "fixed32 log2_e value: " << numbers::log2e_v<fixed32>();
    GTEST_LOG_(INFO) << "fixed32 log10_e value: " << numbers::log10e_v<fixed32>();
    GTEST_LOG_(INFO) << "fixed32 inv_pi value: " << numbers::inv_pi_v<fixed32>();
    GTEST_LOG_(INFO) << "fixed32 inv_sqrtpi value: " << numbers::inv_sqrtpi_v<fixed32>();
    GTEST_LOG_(INFO) << "fixed32 ln2 value: " << numbers::ln2_v<fixed32>();
    GTEST_LOG_(INFO) << "fixed32 ln10 value: " << numbers::ln10_v<fixed32>();
    GTEST_LOG_(INFO) << "fixed32 sqrt2 value: " << numbers::sqrt2_v<fixed32>();
    GTEST_LOG_(INFO) << "fixed32 sqrt3 value: " << numbers::sqrt3_v<fixed32>();
    GTEST_LOG_(INFO) << "fixed32 inv_sqrt3 value: " << numbers::inv_sqrt3_v<fixed32>();
    GTEST_LOG_(INFO) << "fixed32 egamma value: " << numbers::egamma_v<fixed32>();
    GTEST_LOG_(INFO) << "fixed32 phi value: " << numbers::phi_v<fixed32>();

    #ifdef EIRIN_FIXED_HAS_INT128
    // fixed64
    GTEST_LOG_(INFO) << "fixed64 max value: " << max_value<fixed64>() << ", min value: " << min_value<fixed64>();
    GTEST_LOG_(INFO) << "fixed64 e value: " << fixed64::e() << ", pi value: " << fixed64::pi();
    GTEST_LOG_(INFO) << "fixed64 log2_e value: " << numbers::log2e_v<fixed64>();
    GTEST_LOG_(INFO) << "fixed64 log10_e value: " << numbers::log10e_v<fixed64>();
    GTEST_LOG_(INFO) << "fixed64 inv_pi value: " << numbers::inv_pi_v<fixed64>();
    GTEST_LOG_(INFO) << "fixed64 inv_sqrtpi value: " << numbers::inv_sqrtpi_v<fixed64>();
    GTEST_LOG_(INFO) << "fixed64 ln2 value: " << numbers::ln2_v<fixed64>();
    GTEST_LOG_(INFO) << "fixed64 ln10 value: " << numbers::ln10_v<fixed64>();
    GTEST_LOG_(INFO) << "fixed64 sqrt2 value: " << numbers::sqrt2_v<fixed64>();
    GTEST_LOG_(INFO) << "fixed64 sqrt3 value: " << numbers::sqrt3_v<fixed64>();
    GTEST_LOG_(INFO) << "fixed64 inv_sqrt3 value: " << numbers::inv_sqrt3_v<fixed64>();
    GTEST_LOG_(INFO) << "fixed64 egamma value: " << numbers::egamma_v<fixed64>();
    GTEST_LOG_(INFO) << "fixed64 phi value: " << numbers::phi_v<fixed64>();
    #endif
}

#ifdef EIRIN_FIXED_HAS_INT128
TEST(fixed64, operator)
{
    auto fp1 = 0_f64;
    EXPECT_EQ(++fp1, 1_f64);
    fp1 = 1.14_f64;
    EXPECT_EQ(fp1 + 5.14_f64, 6.28_f64);
    EXPECT_EQ(fp1 * 2, 2.28_f64);
    EXPECT_EQ(514_f64 / 2, 257_f64);
    EXPECT_EQ(515_f64 % 2_f64, 1_f64);
    EXPECT_EQ(--fp1, 0.14_f64);

    EXPECT_THROW(
        (void)(1_f64 / 0),
        divide_by_zero
    );
}

TEST(fixed64, rounding)
{
    EXPECT_EQ(round(114.414_f64), 114_f64);
    EXPECT_EQ(round(114.514_f64), 115_f64);
    EXPECT_EQ(round(-114.414_f64), -114_f64);
    EXPECT_EQ(round(-114.514_f64), -115_f64);
}

TEST(fixed64, decompression)
{
    EXPECT_FALSE(signbit(0_f64));
    EXPECT_FALSE(signbit(1_f64));
    EXPECT_TRUE(signbit(-1_f64));

    EXPECT_EQ((1_f64).raw_integral_part(), 1);
    EXPECT_EQ((-1_f64).raw_integral_part(), 2147483647);

    EXPECT_EQ((1_f64).integral_part(), 1);
    EXPECT_EQ((-1_f64).integral_part(), 1);

    EXPECT_EQ((1_f64).fractional_part(), 0);
    EXPECT_EQ((-1_f64).fractional_part(), 0);

    {
        auto val = 1_f64 / 2;
        EXPECT_EQ(val.integral_part(), 0);
        EXPECT_EQ(val.fractional_part(), 1ULL << (val.precision - 1));
    }
}

TEST(fixed64, math)
{
    using test_math::expect_fixed_eq;

    EXPECT_EQ(abs(-114.514_f64), 114.514_f64);
    EXPECT_EQ(sin(0_f64), 0_f64);
    EXPECT_TRUE(expect_fixed_eq(sin(1_f64), 0.841471_f64));
    EXPECT_TRUE(expect_fixed_eq(sin(fixed64::pi() / 6), 0.5_f64));
    EXPECT_TRUE(expect_fixed_eq(cos(0_f64), 1_f64));
    EXPECT_TRUE(expect_fixed_eq(cos(fixed64::pi() / 3), 0.5_f64));
    EXPECT_TRUE(expect_fixed_eq(cos(1_f64), 0.540302_f64));
    EXPECT_EQ(tan(0_f64), 0_f64);
    EXPECT_TRUE(expect_fixed_eq(tan(1_f64), 1.557407_f64));
    EXPECT_TRUE(expect_fixed_eq(atan(0_f64), 0_f64));
    EXPECT_TRUE(expect_fixed_eq(atan(1_f64), 0.785398_f64));
    EXPECT_TRUE(expect_fixed_eq(sqrt(0_f64), 0_f64));
    EXPECT_TRUE(expect_fixed_eq(sqrt(4_f64), 2_f64));
    EXPECT_TRUE(expect_fixed_eq(sqrt(114.514_f64), 10.701121_f64));
    EXPECT_TRUE(expect_fixed_eq(cbrt(0_f64), 0_f64));
    EXPECT_TRUE(expect_fixed_eq(cbrt(1_f64), 1_f64));
    EXPECT_TRUE(expect_fixed_eq(cbrt(8_f64), 2_f64));
    EXPECT_TRUE(expect_fixed_eq(cbrt(27_f64), 3_f64));
    EXPECT_TRUE(expect_fixed_eq(log2(2_f64), 1_f64));
    EXPECT_TRUE(expect_fixed_eq(log2(10_f64), 3.321928_f64));
    EXPECT_TRUE(expect_fixed_eq(log(fixed64::e()), 1_f64));
    EXPECT_TRUE(expect_fixed_eq(log(114.514_f64), 4.740697_f64));
    EXPECT_TRUE(expect_fixed_eq(log10(10_f64), 1_f64));
    EXPECT_TRUE(expect_fixed_eq(log10(114.514_f64), 2.058859_f64));
    EXPECT_TRUE(expect_fixed_eq(exp(1_f64), fixed64::e()));
    EXPECT_EQ(radians(180_f64), numbers::pi_f64);
    EXPECT_EQ(degrees(numbers::pi_f64), 180_f64);
    EXPECT_EQ(radians(90_f64), numbers::pi_f64 / 2);
    EXPECT_EQ(degrees(numbers::pi_f64 / 2), 90_f64);
}
#endif

int main(int argc, char* argv[])
{
    auto fp1 = fixed32(1);
    auto fp2 = fixed32(3);
    fp2 /= 114.514_f32;
    papilio::println("{}, {}, {}", (float)fixed32::e(), (float)fixed32::pi(), (float)fixed32::double_pi());
    papilio::println("{}, {}, {}", (int)fp1, (float)(114.514_f32 / 3), (float)fp2);
    papilio::println("abs = {}, eps = {}, compare = {}", (float)abs(-114.5_f32), (float)fixed32::nearly_compare_epsilon(), 1.114_f32 < 1.115_f32);
    papilio::println("sqrt(114.514) = {}, {}", (float)sqrt(114.514_f32), std::sqrt(114.514));
    papilio::println("sin(pi/6) = {}, {}", (float)sin(fixed32::pi() / 6), std::sin(std::numbers::pi / 6));
    papilio::println("cos(pi/6) = {}, {}", (float)cos(fixed32::pi() / 6), std::cos(std::numbers::pi / 6));
    papilio::println("tan(pi/6) = {}, {}", (float)tan(fixed32::pi() / 6), std::tan(std::numbers::pi / 6));
    papilio::println("atan(1.114) = {}, {}", (float)atan(1.514_f32), std::atan(1.514));
    papilio::println("cbrt(1.114) = {}, {}", (float)cbrt(-114.514_f32), std::cbrt(-114.514));
    papilio::println("log2(10) = {}, {}", (float)log2(10_f32), std::log2(10));
    constexpr auto log2_10 = fixed32::template from_fixed_num_value<60>(0x35269E12F346E200ll);
    (void)log2_10;
    papilio::println("ln(114.514) = {}, {}", (float)log(114.514_f32), std::log(114.514));
    papilio::println("log10(114.514) = {}, {}", (float)log10(114.514_f32), std::log10(114.514));
    papilio::println("exp(4) = {}, {}", (float)exp(4_f32), std::exp(4));
    papilio::println("pow(1.14, 5.14) = {}, {}", (float)pow(1.14_f32, 5.14_f32), std::pow(1.14, 5.14));
    papilio::println("ceil(114.514) = {}, {}", (float)ceil(114.514_f32), std::ceil(114.514));
    papilio::println("ceil(32767.5) = {}, {}", (float)ceil(32767.5_f32), std::ceil(32767.5));
    papilio::println("floor(-114.514) = {}, {}", (float)floor(-114.514_f32), std::floor(-114.514));
    papilio::println("floor(-32767.5) = {}, {}", (float)floor(-32767.5_f32), std::floor(-32767.5));
    papilio::println("round(-114.514) = {}, {}", (float)round(-114.514_f32), std::round(-114.514));
    papilio::println("round(-114.414) = {}, {}", (float)round(-114.414_f32), std::round(-114.414));
    papilio::println("round(-32767.5) = {}, {}", (float)round(-32767.5_f32), std::round(-32767.5));
    papilio::println("round(-32767.4) = {}, {}", (float)round(-32767.4_f32), std::round(-32767.4));
    papilio::println("round(114.514) = {}, {}", (float)round(114.514_f32), std::round(114.514));
    papilio::println("round(114.414) = {}, {}", (float)round(114.414_f32), std::round(114.414));
    papilio::println("round(32767.5) = {}, {}", (float)round(32767.5_f32), std::round(32767.5));
    papilio::println("round(32767.4) = {}, {}", (float)round(32767.4_f32), std::round(32767.4));
    auto modf_b = 1.15;
    (void)modf_b;
    papilio::println("5.14 mod 1.14 = {}, {}", (float)(5.14_f32 % 1.14_f32), std::fmod(5.14, 1.14));
    auto fp = fixed32(0);
    if(f32_from_cstring("-114.514", 8, fp))
        papilio::println("test from_cstring: {}, {}", (float)fp, (float)-114.514_f32);
    if(fixed_from_cstring("495.625", 8, fp))
        papilio::println("test from_cstring: {}, {}", (float)fp, (float)-"495.625"_f32);
    fp = 0_f32;
    papilio::println("test parse: {}, {}, {}", parse("114b.514a", fp), (float)fp, (float)114.514_f32);
    papilio::println("test parse: {}, {}, {}", parse("114.514a", fp), (float)fp, (float)114.514_f32);
    papilio::println("test parse: {}, {}, {}", parse("114a.514a", fp), (float)fp, (float)114.514_f32);

    std::cout << 114.5625_f32 << std::endl;
    std::cout << -114.5625_f32 << std::endl;

    static_assert(papilio::formattable<fixed32>);
    papilio::println("{:f}", 114.5625_f32);

    // util::print_constants();
    // papilio::println("constants used in atan: {:X}, {:X}", util::eval_value<int64_t>(0.2247, 61), util::eval_value<int64_t>(0.0663, 61));
    // auto test = fixed64::template from_fixed_num_value<61>(util::eval_value<int64_t>(0.0063, 61));
    // papilio::println("test: {}", test);

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
