#ifndef EIRIN_FIXED_RANDOM_HPP
#define EIRIN_FIXED_RANDOM_HPP

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <unordered_map>
#include <fstream>
#include <eirin/macro.hpp>
#include <vector>
#include <random>

namespace eirin
{
namespace detail
{
    /**
     * @brief Permuted Congruential Random Generator Variant.
     * PCG is a family of simple fast space-efficient statistically good algorithms for random number generation.
     * Unlike many general-purpose RNGs, they are also hard to predict.
     * And in this instance, we use a state counter.
     * 
     * @tparam UIntType unsigned int type, use for parameters and return type.
     * @tparam state initial state.
     * @tparam multiplier 
     * @tparam increment 
     * @tparam mix_multiplier 
     * @tparam xor_multiplier 
     */
    template <class UIntType, UIntType state, UIntType multiplier, UIntType increment, UIntType mix_multiplier, UIntType xor_multiplier>
    class permuted_congruential_engine
    {
    protected:
        UIntType m_seed;
        UIntType m_state;

        EIRIN_ALWAYS_INLINE UIntType mix_first(UIntType x)
        {
            x >>= 8 ^ x;
            x += increment;
            return x;
        }

        EIRIN_ALWAYS_INLINE UIntType _xor(UIntType x)
        {
            x = x * 0x100 ^ x;
            x *= xor_multiplier;
            return x;
        }

        EIRIN_ALWAYS_INLINE UIntType mix_second(UIntType x)
        {
            x >>= 8 ^ x;
            x *= mix_multiplier;
            return x;
        }

        EIRIN_ALWAYS_INLINE UIntType generate()
        {
            result_type val = m_state * multiplier + m_seed;
            val = mix_second(_xor(mix_first(val)));
            ++m_state;
            return (val & 0x7FFFFFFE) ^ (val >> 8);
        }

    public:
        using result_type = UIntType;

        permuted_congruential_engine(UIntType seed)
            : m_seed(seed), m_state(state) {}

        result_type operator()()
        {
            return generate();
        }

        void seed(UIntType seed)
        {
            m_seed = seed;
        }
    };
} // namespace detail

typedef detail::permuted_congruential_engine<uint32_t, 0x0, 0xB5297A4D, 0x68E31DA4, 0x1B56C4E9, 0x92D68CA2> pcg2014;

typedef detail::permuted_congruential_engine<uint64_t, 0x0, 0x5851F42D4C957F2D, 0x9E3779B97F4A7C15, 0x94D049BB133111EB, 0xBF58476D1CE4E5B9> pcg2014_64;

/**
 * @brief Temporary test function for random number generator, remove it in the future.
 * 
 */
inline void test()
{
    auto seed = 114514u;
    std::unordered_map<int32_t, uint32_t> rands;
    // open a file stream to write the random results
    auto fs = std::ofstream("random_res.txt", std::ios::out);
    pcg2014 rng(seed);
    std::random_device rd;
    std::mt19937 mt(rd());
    for(int i = 0; i < 114514; ++i)
    {
        auto val = rng();
        rng.seed(val);
        rands[val]++;
        if(fs.is_open())
        {
            fs << "random " << i << " for seed " << seed << " " << val << '\n';
        }
        else
            printf("random %d for seed %u %u\n", i, seed, val);
    }
    fs.close();
    std::vector<std::pair<int32_t, int32_t>> pairs;
    pairs.reserve(rands.size());
    fs.open("random_res_summary.txt", std::ios::out);
    for(const auto& [key, value] : rands)
    {
        pairs.push_back(std::make_pair(key, value));
    }
    std::sort(pairs.begin(), pairs.end(), [](const auto& a, const auto& b)
              { return a.second > b.second; });
    for(const auto& [key, value] : pairs)
    {
        if(fs.is_open())
        {
            fs << "value " << key << " appears " << value << " times\n";
        }
    }
    fs.close();
}
} // namespace eirin

#endif
