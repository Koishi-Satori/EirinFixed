#ifndef EIRIN_FIXED_EXT_UNIFORM_DIST_HPP
#define EIRIN_FIXED_EXT_UNIFORM_DIST_HPP

#pragma once
#ifdef EIRIN_OS_WINDOWS
// C4244: conversion from 'type1' to 'type2', possible loss of data
// This is excepted, so disable it.
#pragma warning(disable : 4244)
#endif

#include <random>
#include <eirin/fixed.hpp>

namespace eirin
{
template <typename FixedType, typename DistType>
requires is_fixed_point_v<FixedType>
class fixed_int_distribution_adapter
{
public:
    typedef FixedType result_type;
    typedef typename result_type::value_type value_type;
    typedef std::make_unsigned_t<value_type> unsigned_type;
    typedef DistType underlying_type;

    struct param_type
    {
        using target_type = typename DistType::param_type;

        explicit param_type(const result_type& a = std::numeric_limits<result_type>::min(), const result_type& b = std::numeric_limits<result_type>::max())
            : m_a(a), m_b(b)
        {
            assert(a <= b);
        }

        result_type a() const
        {
            return m_a;
        }

        result_type b() const
        {
            return m_b;
        }

        friend bool operator==(const param_type& lhs, const param_type& rhs) noexcept
        {
            return lhs.m_a == rhs.m_a && lhs.m_b == rhs.m_b;
        }

        friend bool operator!=(const param_type& lhs, const param_type& rhs) noexcept
        {
            return !(lhs == rhs);
        }

        target_type to_underlying() const
        {
            return target_type(static_cast<value_type>(m_a.internal_value()), static_cast<value_type>(m_b.internal_value()));
        }

    private:
        result_type m_a;
        result_type m_b;
    };

    /**
     * @brief Construct a new uniform fixed distribution object, with given range [a, b].
     * 
     * @param a 
     * @param b 
     */
    explicit fixed_int_distribution_adapter(const FixedType& a = std::numeric_limits<FixedType>::min(), const FixedType& b = std::numeric_limits<FixedType>::max())
        : m_dist(static_cast<value_type>(a.internal_value()), static_cast<value_type>(b.internal_value())), m_param(a, b)
    {}

    explicit fixed_int_distribution_adapter(const param_type& param)
        : m_dist(static_cast<value_type>(param.a().internal_value()), static_cast<value_type>(param.b().internal_value())), m_param(param)
    {}

    template <typename _UniformRandomBitGenerator>
    FixedType operator()(_UniformRandomBitGenerator& g)
    {
        return operator()(g, m_param);
    }

    template <typename _UniformRandomBitGenerator>
    FixedType operator()(_UniformRandomBitGenerator& g, const param_type& param)
    {
        // the m_param and the underlying distribution should be consistent, so just call m_dist.operator(g)
        auto raw_val = m_dist(g);
        return FixedType::template from_fixed_num_value<result_type::precision>(static_cast<value_type>(raw_val));
    }

    param_type param() const noexcept
    {
        return m_param;
    }

    void param(const param_type& param)
    {
        m_param = param;
        m_dist = std::uniform_int_distribution<value_type>(static_cast<value_type>(param.a().internal_value()), static_cast<value_type>(param.b().internal_value()));
    }

    result_type a() const
    {
        return m_param.a();
    }

    result_type b() const
    {
        return m_param.b();
    }

    result_type min() const
    {
        return this->a();
    }

    result_type max() const
    {
        return this->b();
    }

    void reset() noexcept
    {
        // Nothing to do for uniform distribution.
    }

private:
    // The underlying int distribution.
    // To be noticed that we use the internal representation of the fixed number.
    underlying_type m_dist;
    param_type m_param;
};

template <typename FixedType, typename DistType>
requires is_fixed_point_v<FixedType>
class fixed_distribution_adapter
{
public:
    typedef FixedType result_type;
    typedef typename result_type::value_type value_type;
    typedef std::make_unsigned_t<value_type> unsigned_type;
    typedef DistType underlying_type;

    struct param_type
    {
        using target_type = typename DistType::param_type;

        explicit param_type(const result_type& a = std::numeric_limits<result_type>::min(), const result_type& b = std::numeric_limits<result_type>::max())
            : m_a(a), m_b(b)
        {
            assert(a <= b);
        }

        result_type a() const
        {
            return m_a;
        }

        result_type b() const
        {
            return m_b;
        }

        friend bool operator==(const param_type& lhs, const param_type& rhs) noexcept
        {
            return lhs.m_a == rhs.m_a && lhs.m_b == rhs.m_b;
        }

        friend bool operator!=(const param_type& lhs, const param_type& rhs) noexcept
        {
            return !(lhs == rhs);
        }

        target_type to_underlying() const
        {
            return target_type(static_cast<value_type>(m_a.internal_value()), static_cast<value_type>(m_b.internal_value()));
        }

    private:
        result_type m_a;
        result_type m_b;
    };

    /**
     * @brief Construct a new uniform fixed distribution object, with given range [a, b].
     * 
     * @param a 
     * @param b 
     */
    explicit fixed_distribution_adapter(const FixedType& a = std::numeric_limits<FixedType>::min(), const FixedType& b = std::numeric_limits<FixedType>::max())
        : m_dist(static_cast<value_type>(a.internal_value()), static_cast<value_type>(b.internal_value())), m_param(a, b)
    {}

    explicit fixed_distribution_adapter(const param_type& param)
        : m_dist(static_cast<value_type>(param.a().internal_value()), static_cast<value_type>(param.b().internal_value())), m_param(param)
    {}

    template <typename _UniformRandomBitGeneratorAdapter>
    FixedType operator()(_UniformRandomBitGeneratorAdapter& a)
    {
        return operator()(a, m_param);
    }

    template <typename _UniformRandomBitGeneratorAdapter>
    FixedType operator()(_UniformRandomBitGeneratorAdapter& a, const param_type& param)
    {
        // the m_param and the underlying distribution should be consistent, so just call m_dist.operator(g)
        auto raw_val = m_dist(a.__underlying_engine());
        return FixedType::template from_fixed_num_value<result_type::precision>(static_cast<value_type>(raw_val));
    }

    param_type param() const noexcept
    {
        return m_param;
    }

    void param(const param_type& param)
    {
        m_param = param;
        m_dist = underlying_type(static_cast<value_type>(param.a().internal_value()), static_cast<value_type>(param.b().internal_value()));
    }

    result_type a() const
    {
        return m_param.a();
    }

    result_type b() const
    {
        return m_param.b();
    }

    result_type min() const
    {
        return this->a();
    }

    result_type max() const
    {
        return this->b();
    }

    void reset() noexcept
    {
        // Nothing to do for uniform distribution.
    }

private:
    // The underlying distribution.
    // To be noticed that we use the internal representation of the fixed number.
    underlying_type m_dist;
    param_type m_param;
};
} // namespace eirin

#endif
