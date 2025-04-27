Mathematic Functions
======================

Next listed are the mathematical functions provided by the library. The functions are pure C++ implementations with high precision and high performance, and implemented in the ``math.hpp`` header file.

The functions are implemented as template functions, which means that they can be used with any fixed point type provided by this library.

- min and max
    - min_value
    - max_value
- rounding functions
    - ceil/floor
    - trunc
    - round
- absolute value
    - abs
- square root
    - sqrt
- cube root
    - cbrt
- trigonometric functions
    - sin
    - cos
    - tan
    - atan
- logarithmic functions
    - log
    - log2
    - log10
- exponential functions
    - exp
    - pow

.. note::
    The implementation of max/min value functions used ``std::numeric_limits`` to get the max/min value of the fixed point type, so you need to make sure the store type has specialization for ``std::numeric_limits``.
    If not, the ``std::numeric_limits`` specialization of the fixed point type will calculate the max/min value with next formula:
    - For the maximum value: ``~static_cast<Type>(1) << (sizeof(Type) * 8 - 1)``
    - For the minimum value: ``static_cast<Type>(1) << (sizeof(Type) * 8 - 1)``
    Then, the ``log`` and ``log10`` functions are calculated based on the formula of change of base of logarithms, and the ``log2`` function is implemented first. Because there exists a fast binary logarithm used some tricks to calculate log2 faster.

.. code-block:: c++

    #include <iostream>
    #include <eirin/fixed.hpp>
    #include <math.hpp>

    int main()
    {
        using namespace eirin;

        std::cout << "fixed64 max value: " << max_value<fixed64>() << std::endl;
        std::cout << "fixed32 min value: " << min_value<fixed32>() << std::endl;

        std::cout << "sqrt(114514): " << sqrt(114514_f64) << std::endl;

        std::cout << "log2(514): " << log2(514_f32) << std::endl;

        return 0;
    }