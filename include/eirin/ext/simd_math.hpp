#ifndef EIRIN_FIXED_EXT_SIMD_MATH_HPP
#define EIRIN_FIXED_EXT_SIMD_MATH_HPP

#pragma once

#include <eirin/macro.hpp>
#include <eirin/fixed.hpp>
#include "eirin/numbers.hpp"
#include <cmath>
#include <bitset>

#ifdef EIRIN_PLATFORM_HAS_SIMD
#    include <immintrin.h>
#    include <version>

#    ifdef EIRIN_PLATFORM_SIMD_AVX

#    endif

#endif

#ifdef EIRIN_ARCH_X86
#    include <cpuid.h>
#endif

namespace eirin::simd
{
class platform_support
{
    static inline bool features_detected = false;

    // we only consider to support x86 for now.
    // a bit set for storeage.
    static inline std::bitset<8> features_flag = 0x00;

    EIRIN_ALWAYS_INLINE static void detect_features()
    {
        if(features_detected) [[likely]]
            return;
        features_detected = true;
#ifdef EIRIN_ARCH_X86
#    if defined(__GNUC__) || defined(__clang__)
        u_int32_t eax, ebx, ecx, edx;
        __cpuid(1, eax, ebx, ecx, edx);
        features_flag.set(0, (edx & (1 << 26)) != 0);
        features_flag.set(1, (ecx & (1 << 20)) != 0);
        features_flag.set(2, (ecx & (1 << 28)) != 0);
        __cpuid_count(7, 0, eax, ebx, ecx, edx);
        features_flag.set(3, (ebx & (1 << 5)) != 0);
        features_flag.set(4, (ebx & (1 << 16)) != 0);
        features_flag.set(5, (ebx & (1 << 17)) != 0);
#    elif defined(_MSC_VER)
        int cpu_info[4];
        __cpuid(cpu_info, 1);
        uint32_t ecx = cpu_info[2];
        uint32_t edx = cpu_info[3];
        features_flag.set(0, (edx & (1 << 26)) != 0);
        features_flag.set(1, (ecx & (1 << 20)) != 0);
        features_flag.set(2, (ecx & (1 << 28)) != 0);
        __cpuidex(cpu_info, 7, 0);
        uint32_t ebx = cpu_info[1];
        features_flag.set(3, (ebx & (1 << 5)) != 0);
        features_flag.set(4, (ebx & (1 << 16)) != 0);
        features_flag.set(5, (ebx & (1 << 17)) != 0);
#    else
        return;
#    endif
#else
        return;
#endif
    }

public:
    inline static bool supports_sse2()
    {
        detect_features();
        return features_flag.test(0);
    }

    inline static bool supports_sse4_2()
    {
        detect_features();
        return features_flag.test(1);
    }

    inline static bool supports_avx()
    {
        detect_features();
        return features_flag.test(2);
    }

    inline static bool supports_avx2()
    {
        detect_features();
        return features_flag.test(3);
    }

    inline static bool supports_avx512_f()
    {
        detect_features();
        return features_flag.test(4);
    }

    inline static bool supports_avx512_dq()
    {
        detect_features();
        return features_flag.test(5);
    }
};

namespace detail
{
    EIRIN_ALWAYS_INLINE __m256i avx_mm256_mullo_epi64(__m256i __A, __m256i __B)
    {
#ifdef __AVX512DQ__
        // For machines with AVX-512, we just calc (a * b) >> 32.
        return _mm256_srli_epi64(_mm256_mullo_epi64(__A, __B), 32);
#else
        // Fallback implementation for lane-wise 64-bit multiplication without AVX-512.
        // Compute sign masks (0 or -1) from sign bit using shifts to avoid
        // relying on potentially unavailable 64-bit compare intrinsics.
        __m256i sign_bit_a = _mm256_srli_epi64(__A, 63);
        __m256i sign_mask_a = _mm256_sub_epi64(_mm256_setzero_si256(), sign_bit_a);
        __m256i sign_bit_b = _mm256_srli_epi64(__B, 63);
        __m256i sign_mask_b = _mm256_sub_epi64(_mm256_setzero_si256(), sign_bit_b);

        __m256i abs_a = _mm256_sub_epi64(_mm256_xor_si256(__A, sign_mask_a), sign_mask_a);
        __m256i abs_b = _mm256_sub_epi64(_mm256_xor_si256(__B, sign_mask_b), sign_mask_b);

        const __m256i low_mask = _mm256_set1_epi64x(0xFFFFFFFFULL);
        __m256i a_lo = _mm256_and_si256(abs_a, low_mask);
        __m256i a_hi = _mm256_srli_epi64(abs_a, 32);
        __m256i b_lo = _mm256_and_si256(abs_b, low_mask);
        __m256i b_hi = _mm256_srli_epi64(abs_b, 32);

        __m256i lo_lo = _mm256_mul_epu32(a_lo, b_lo);
        __m256i hi_hi = _mm256_mul_epu32(a_hi, b_hi);
        __m256i hi_lo = _mm256_mul_epu32(a_hi, b_lo);
        __m256i lo_hi = _mm256_mul_epu32(a_lo, b_hi);

        // we have [hi_hi, (hi_lo + lo_hi), lo_lo] here as 128 bit rep.
        // then for sim fixed point mult, we need to right shift 32 bits.
        __m256i mid = _mm256_add_epi64(hi_lo, lo_hi);
        // so the result is (hi_hi << 32) + (hi_lo + lo_hi) + (lo_lo >> 32).
        __m256i result = _mm256_add_epi64(_mm256_add_epi64(mid, _mm256_slli_epi64(hi_hi, 32)), _mm256_srli_epi64(lo_lo, 32));

        // restore sign
        __m256i sign = _mm256_xor_si256(sign_mask_a, sign_mask_b);
        return _mm256_sub_epi64(_mm256_xor_si256(result, sign), sign);
#endif
    }

    EIRIN_ALWAYS_INLINE __m256i _mm256_fpint_epi64(__m256i __X)
    {
        // Check sign bit (bit 63)
        __m256i sign_bit = _mm256_srli_epi64(__X, 63);
        __m256i sign_mask = _mm256_sub_epi64(_mm256_setzero_si256(), sign_bit);

        // For negative numbers: compute two's complement
        // For positive numbers: keep as is
        __m256i abs_val = _mm256_sub_epi64(_mm256_xor_si256(__X, sign_mask), sign_mask);

        // Right shift by 32 to get integer part (dropping fractional bits).
        // Then we left shift 32 bits to get fixed point value.
        __m256i integer_part = _mm256_slli_epi64(_mm256_srli_epi64(abs_val, 32), 32);

        // Restore sign for the integer part
        return _mm256_sub_epi64(_mm256_xor_si256(integer_part, sign_mask), sign_mask);
    }

    template <bool fixed_rep = true>
    EIRIN_ALWAYS_INLINE void debug_m256i_log(__m256i x, const char* var_name)
    {
        int64_t x1 = _mm256_extract_epi64(x, 0), x2 = _mm256_extract_epi64(x, 1), x3 = _mm256_extract_epi64(x, 2), x4 = _mm256_extract_epi64(x, 3);
        if constexpr(fixed_rep)
        {
            std::cout << "DEBUG __m256i " << var_name << " fixed point value: [" << fixed64::from_internal_value(x1) << ',';
            std::cout << fixed64::from_internal_value(x2) << ',';
            std::cout << fixed64::from_internal_value(x3) << ',';
            std::cout << fixed64::from_internal_value(x4) << ']' << std::endl;
        }
        else
        {
            printf("DEBUG __m256i %s fixed point value:[%lx, %lx, %lx, %lx]\n", var_name, x1, x2, x3, x4);
        }
    }

    EIRIN_ALWAYS_INLINE __m256i __avx__simd_reduce_angle(__m256i x)
    {
        // these [4 * i64] vector stores the fixed point internal represent.
        static const __m256i two_pi = _mm256_set1_epi64x((numbers::pi_f64 * 2_f64).internal_value());
        static const __m256i inv_two_pi = _mm256_set1_epi64x((1_f64 / (numbers::pi_f64 * 2_f64)).internal_value());
        // n = floor(x / (2pi))
        __m256i n = _mm256_fpint_epi64(avx_mm256_mullo_epi64(x, inv_two_pi));
        // x = x - n * 2pi
        return _mm256_sub_epi64(x, avx_mm256_mullo_epi64(n, two_pi));
    }

    // EIRIN_ALWAYS_INLINE __m256i __avx__simd_fpsin(__m256 x)
    // {
    //     static const __m256i threshold = _mm256_set1_epi64x((0.00015_f64).internal_value());
    // }

    EIRIN_ALWAYS_INLINE __m128i __sse_simd_reduce_angle(__m128i x)
    {
        // these [2 * i64] vector stores the fixed point internal represent.
        static constexpr int64_t two_pi_v[2] = {(numbers::pi_f64 * 2_f64).internal_value(), (numbers::pi_f64 * 2_f64).internal_value()};
        static constexpr int64_t inv_two_pi_v[2] = {(1_f64 / (numbers::pi_f64 * 2_f64)).internal_value(), (1_f64 / (numbers::pi_f64 * 2_f64)).internal_value()};
        static const __m128i two_pi = _mm_lddqu_si128((__m128i*)two_pi_v);
        static const __m128i inv_two_pi = _mm_lddqu_si128((__m128i*)inv_two_pi_v);
        // n = x / (2pi)
        __m128i n = _mm_mul_epi32(x, inv_two_pi);
        // x = x - n * (2pi)
        return _mm_sub_epi64(x, _mm_mul_epi32(n, two_pi));
    }
} // namespace detail

#define FIXED64(X) fixed64::from_internal_value(X)

EIRIN_ALWAYS_INLINE std::array<fixed64, 4> simd_reduce_angle(std::array<fixed64, 4> xs) noexcept
{
    __m256i xv = _mm256_set_epi64x(xs[3].internal_value(), xs[2].internal_value(), xs[1].internal_value(), xs[0].internal_value());
    __m256i rv = detail::__avx__simd_reduce_angle(xv);
    int64_t r1 = _mm256_extract_epi64(rv, 0), r2 = _mm256_extract_epi64(rv, 1), r3 = _mm256_extract_epi64(rv, 2), r4 = _mm256_extract_epi64(rv, 3);
    return {FIXED64(r1), FIXED64(r2), FIXED64(r3), FIXED64(r4)};
}

EIRIN_ALWAYS_INLINE std::array<fixed64, 2> simd_reduce_angle_2(std::array<fixed64, 2> xs) noexcept
{
    __m128i xv = _mm_lddqu_si128((__m128i*)xs.data());
    __m128i rv = detail::__sse_simd_reduce_angle(xv);
    int64_t r1 = _mm_extract_epi64(rv, 0b1), r2 = _mm_extract_epi64(rv, 0b0);
    return {FIXED64(r1), FIXED64(r2)};
}

#undef FIXED64
} // namespace eirin::simd
#endif
