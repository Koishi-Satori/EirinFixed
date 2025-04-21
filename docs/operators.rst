Arithmetic Operators
=======================

Quadratic Operators
--------------------

You can perform quadratic operations and module between fixed and fixed, fixed and integral, or integral and fixed.

The output will always be fixed point.

.. code-block:: c++

    #include <iostream>
    #include <eirin/fixed.hpp>
    
    int main() {
        using namespace eirin;
        fixed32 fp1 = "123.45"_f32;
        fixed32 fp2 = "678.90"_f32;
        int i = 10;

        std::cout << "fp1 + fp2: " << (fp1 + fp2) << std::endl;
        std::cout << "fp1 - fp2: " << (fp1 - fp2) << std::endl;
        std::cout << "fp1 * i: " << (fp1 * i) << std::endl;
        std::cout << "i * fp2: " << (i * fp2) << std::endl;
        std::cout << "fp1 / i: " << (fp1 / i) << std::endl;
        std::cout << "i / fp2: " << (i / fp2) << std::endl;

        return 0;
    }

Self Increment/Decrement Operators
--------------------------

You can perform self increment and decrement operations on fixed point numbers.

.. code-block:: c++

    #include <iostream>
    #include <eirin/fixed.hpp>
    
    int main() {
        using namespace eirin;
        fixed32 fp1 = "123.45"_f32;

        std::cout << "fp1: " << fp1 << std::endl;
        std::cout << "fp1++: " << (fp1++) << std::endl;
        std::cout << "++fp1: " << (++fp1) << std::endl;
        std::cout << "fp1--: " << (fp1--) << std::endl;
        std::cout << "--fp1: " << (--fp1) << std::endl;

        return 0;
    }

Cast Operators
=======================

You can cast a fixed point number to an integral or floating point type.

.. code-block:: c++

    #include <iostream>
    #include <eirin/fixed.hpp>
    
    int main() {
        using namespace eirin;
        fixed32 fp1 = "123.45"_f32;

        std::cout << "fp1: " << fp1 << std::endl;
        std::cout << "fp1 to int: " << static_cast<int>(fp1) << std::endl;
        std::cout << "fp1 to float: " << static_cast<float>(fp1) << std::endl;

        return 0;
    }

Comparison Operators
=======================

You can compare fixed point numbers with comparison operators.

.. code-block:: c++

    #include <iostream>
    #include <eirin/fixed.hpp>
    
    int main() {
        using namespace eirin;
        fixed32 fp1 = "123.45"_f32;
        fixed32 fp2 = "678.90"_f32;

        std::cout << "fp1: " << fp1 << std::endl;
        std::cout << "fp2: " << fp2 << std::endl;

        std::cout << "fp1 == fp2: " << (fp1 == fp2) << std::endl;
        std::cout << "fp1 != fp2: " << (fp1 != fp2) << std::endl;
        std::cout << "fp1 < fp2: " << (fp1 < fp2) << std::endl;
        std::cout << "fp1 <= fp2: " << (fp1 <= fp2) << std::endl;
        std::cout << "fp1 > fp2: " << (fp1 > fp2) << std::endl;
        std::cout << "fp1 >= fp2: " << (fp1 >= fp2) << std::endl;

        return 0;
    }

You can perform nearly compare with functions ``nearly_eq``, ``nearly_ne``, ``nearly_lt``, ``nearly_lt``, ``nearly_gt_eq``, and ``nearly_lt_eq``.

.. code-block:: c++

    #include <iostream>
    #include <eirin/fixed.hpp>
    
    int main() {
        using namespace eirin;
        fixed32 fp1 = "123.45"_f32;
        fixed32 fp2 = "678.90"_f32;

        std::cout << "fp1: " << fp1 << std::endl;
        std::cout << "fp2: " << fp2 << std::endl;

        std::cout << "fp1 nearly_eq fp2: " << (fp1.nearly_eq(fp2)) << std::endl;
        std::cout << "fp1 nearly_ne fp2: " << (fp1.nearly_ne(fp2)) << std::endl;
        std::cout << "fp1 nearly_lt fp2: " << (fp1.nearly_lt(fp2)) << std::endl;
        std::cout << "fp1 nearly_lt_eq fp2: " << (fp1.nearly_lt_eq(fp2)) << std::endl;
        std::cout << "fp1 nearly_gt fp2: " << (fp1.nearly_gt(fp2)) << std::endl;
        std::cout << "fp1 nearly_gt_eq fp2: " << (fp1.nearly_gt_eq(fp2)) << std::endl;

        return 0;
    }
