# EirinFixed

- Other language: [中文](README.zh-CN.md)

A flexible and high-performance C++ fixed point number library, provides fixed point template class, high precision mathematical operations and basic input and output functions. You can run the benchmarks ```fixed.benchmark``` and ```double.benchmark``` to test for performance differences between the fixed types and the C++ double. Also the benchmark results have been provided in the benchmark directory, running in AMD laptop CPU R7-7735H, with [-O3](benchmark/fixed_benchmark_O3.txt) and [-O2](benchmark/fixed_benchmark_O2.txt) optimization.
The output functions support the formatter provided by [Papilio Charontis](https://github.com/HenryAWE/PapilioCharontis).

It also provides a pre-defined 32bit-width fixed point, with 16bit precision(```fixed32```), and 64bit-width fixed point, with 32bit precision(```fixed64```).
The fixed points require same calculation result in different platforms, devices, operator systems and compilers, and this library fulfills this requirement.
Notice that the fixed64 uses some int128 compiler extension as its IntermediateType, and some compiler might not support it. The ```__msvc_int128.hpp``` in MSVC provides ```std::_Signed128``` and ```std::_Unsigned128```, Clang and GCC in Linux provide ```__int128```.


### Create Fixed Point

You can create a fixed point number with integral or floating types using constructor and literals.

- The literals now only provides for fixed32 and fixed64.
- The way of constructing from floating type is not that recommended.
    - Reason: Floating point error in different platforms, devices, operator systems and compilers.
    - The fixed point needs to make sure same result in different situations.
```c++
#include <eirin/fixed.hpp>

using namespace eirin;

int main(int argc, char** argv)
{
    auto fp32_1 = fixed32(495);
    auto fp32_2 = fixed32(114.514); // using "114.5"_f32 is better.
    auto fp1 = fixed_num<int32_t, int64_t, 20, false>(5);
    auto fp2 = fixed_num<int32_t, int64_t, 20, false>(5.14);
    fp32_1 = 114_f32;
    fp32_2 = 5.14_f32;
    fp32_1 = "49.5"_f32;
    fp32_2 = "114.5"_f32; // multi-platforms work-well!
    fp32_2 = -"114.5"_f32;
    fp32_2 = "-114.5"_f32; // both this and the one above are well.
    return 0;
}
```

You can also create a fixed point from std::basic_istream or strings.

- ```f32_from_cstring``` and ```fixed_from_cstring``` will return true on success.
- the ```parse``` functions needs to pass the end of the string.
    - it will stops when meeting the **first char** of the end string.
    - for example, 114a.514a will only parse "114" part.

```c++
#include <eirin/fixed.hpp>

using namespace eirin;

int main(int argc, char** argv)
{
    fp1 = 0_f32;
    fp2 = fixed_num<int32_t, int64_t, 20, false>(0);
    f32_from_cstring("-114.514", 8, fp1);
    fixed_from_cstring("-5", 2, fp2);
    std::cin >> fp;
    parse("114a.514a", "a", fp);
    return 0;
}
```

### Fixed Point Output

You can use std::ostream or format functions provided by [Papilio Charontis](https://github.com/HenryAWE/PapilioCharontis).

- When using Papilio Charontis, please **make sure** you have include the ```fixed_formatter.hpp``` header file.


```c++
#include <eirin/fixed.hpp>
#include <eirin/ext/papilio_integration.hpp.hpp>

using namespace eirin;

int main(int argc, char** argv)
{
    std::cout << "114.5625"_f32 << std::endl;
    std::cout << "-114.5625"_f32 << std::endl;
    papilio::println("{:s}", 114.5625_f32);
    papilio::println("{:i}", 114.5625_f32);
    papilio::println("{:f}", 114.5625_f32);
    return 0;
}
```

### From Internal Value

- **NOTE: this is not recommended, UNLESS you know what you're doing.**

```c++
#include <eirin/fixed.hpp>

using namespace eirin;

int main(int argc, char** argv)
{
    auto fp = fixed32::from_internal_value(7504789);
    auto log2_10 = fixed32::template from_fixed_num_value<60>(0x35269E12F346E200ll);
    return 0;
}
```

### Fixed Operator Functions

You can convert fixed point to integral or floating types.

```c++
#include <eirin/fixed.hpp>

using namespace eirin;

(int) "114.5"_f32;
(float) "114.5"_f32;
```

You can perform quadratic operations and module between fixed and fixed, fixed and integral, or integral and fixed.
The output will always be fixed point.

```c++
#include <eirin/fixed.hpp>

using namespace eirin;

fp1 = "114.5"_f32;
fp2 = "514"_f32;
fp1 + fp2;
fp1 + 100;
100 + fp2;
fp1 *= 2;
fp1 /= fp2;
```

The compare functions are just like normal compare.

### Fixed Mathematical Functions

Provides high precision and fast pure c++ implemention.

The pre-defined math constants are defined in the fixed point number class.

References:
- Trigonometric Functions: [Efficient Approximations for the Arctangent Function](https://ieeexplore.ieee.org/document/1628884)
- Square Root: [Fixed point sqrt](https://groups.google.com/g/comp.lang.c/c/IpwKbw0MAxw)
- Binary Logarithm: [A Fast Binary Logarithm Algorithm](http://www.claysturner.com/dsp/BinaryLogarithm.pdf)

Supported functions:
- rounding functions
    - ceil/floor
    - trunc
    - round
- abs
- min/max
- sqrt
- trigonometric functions
    - sin/cos/tan
    - asin/acos/atan
- cbrt
- log2/log/log10
- pow/exp

### Supported Compilers

Requires at least C++20.

- xmake >= v2.2.2
- Any C++ compiler that supports C++20

## License
[MIT](LICENSE) License
