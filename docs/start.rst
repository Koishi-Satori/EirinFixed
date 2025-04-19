Supported Platforms
===================

- xmake >= v2.2.2
- C++ compiler that supports C++20
- Boost >= 1.45.0

Currently, the following platforms and compilers are officially tested.

Windows MSVC compiler will be tested later, yet it works well in my another game-engine project (not published yet).

+-------------+---------------------+
| Platform    | Compiler            |
+=============+=====================+
| Windows x64 | MSVC 19.41+         |
+-------------+---------------------+
| Linux x64   | GCC 14              |
+-------------+---------------------+
| Linux x64   | Clang 18            |
+-------------+---------------------+

.. note::
    The 64-bit fixed point number uses `boost::multiprecision::int128_t` as the intermediate type, in GCC 14, the `int128_t` is implemented as `__int128` type. And in MSVC, it is software implemented, so the performance is not as good as GCC and Clang.

Integrate into Your Project
===========================

A. Copy into Your Project
-------------------------

The core library of EirinFixed is a header-only library, you can copy the ``include`` directory into your project, and include the header file in your code.

To be noticed that the header files in ``ext`` directory is not a header-only library, and it includes the interface used for binding fixed point numbers to AngelScript for my another project. If you don't need it, you can ignore it.

If your project has a custom location of ``boost/multiprecision/cpp_int.hpp``, you can include it before including the ``fixed.hpp`` header file, and the library will use your custom location instead of the default location.

.. note::
    **About `fixed_formatter.hpp`**

    This file is used for formatting the fixed point number with ``PapilioFormat`` library, which is a C++20 format library.
    If you want to use it, you need to include the ``PapilioFormat`` library in your project.
    If you don't need it, you can ignore it.

.. note::
    **About The Extension Library**

    It is used to binding a construct function for fixed point numbers to AngelScript.
    The file in `ext` directory is not header-only, you need to copy necessary source files as well.

B. Build and Install with XMake
-------------------------------

The library is built with `xmake`, you can use the following command to build and install the library.

.. code-block:: sh
    xmake f -c
    xmake
    xmake install
