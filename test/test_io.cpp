#include <gtest/gtest.h>
#include <eirin/eirin.hpp>
#include <eirin/io/format.hpp>


#ifdef EIRIN_HAS_LIB_FORMAT

TEST(Fixed32, StdFormat)
{
    using namespace eirin;

    EXPECT_EQ(std::format("{:?}", 0_f32), "0");
    EXPECT_EQ(std::format("{:#?}", -1_f32), "-0x10000");

    EXPECT_EQ(std::format("{:d}", 0_f32), "0");
    EXPECT_EQ(std::format("{:d}", -1_f32), "-1");

    EXPECT_EQ(std::format("{}", 0_f32), "0");
    EXPECT_EQ(std::format("{}", 1_f32), "1");

    EXPECT_EQ(std::format("{:#04x}", 15_f32), "0x0f");
    EXPECT_EQ(std::format("{:#04X}", 15_f32), "0X0F");

    {
        auto val = 1_f32 / 2;
        EXPECT_EQ(std::format("{}", val), "0.5");
        EXPECT_EQ(std::format("{:g}", val), "0.5");
        EXPECT_EQ(std::format("{:f}", val), "0.500000");
        EXPECT_EQ(std::format("{:.2f}", val), "0.50");
    }

    EXPECT_EQ(std::format("{:d}", fixed32::pi()), "3");
    EXPECT_EQ(std::format("{:.2g}", fixed32::pi()), "3.14");
    EXPECT_EQ(std::format("{:.2f}", fixed32::pi()), "3.14");
    EXPECT_EQ(std::format("{:.4f}", fixed32::pi()), "3.1415");
}

#endif

TEST(Fixed32, OStream)
{
    using namespace eirin;

    auto ss_str = [](fixed32 val) -> std::string
    {
        std::ostringstream ss;
        ss << val;
        return std::move(ss).str();
    };

    EXPECT_EQ(ss_str(0_f32), "0");
}
