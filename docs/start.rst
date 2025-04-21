Supported Platforms
===================

- xmake >= v2.2.2
- Any C++ compiler that supports C++20
- Boost >= 1.45.0

Currently, the following platforms and compilers are officially tested.

.. note::
    The 64-bit fixed point number uses `boost::multiprecision::int128_t` as the intermediate type, in GCC 14, the `int128_t` is implemented as `__int128` type. And in MSVC, it is software implemented, so the performance is not as good as GCC and Clang.
    And also, MSVC is not recommended yet, for the performance issues. I recommend to use clang-cl for the best performance, and it is 30x faster than MSVC in my test.

Integrate into Your Project
===========================

A. Copy into Your Project
-------------------------

The core library of EirinFixed is a header-only library, you can copy the ``include`` directory into your project, and include the header file in your code.

To be noticed that the header file ``parse.hpp`` is not a header-only library, and it includes the interface used for binding fixed point numbers to AngelScript for my another project. If you don't need it, you can ignore it.

If your project has a custom location of ``boost/multiprecision/cpp_int.hpp``, you can include it before including the ``fixed.hpp`` header file, and the library will use your custom location instead of the default location.

.. note::
    **About `papilio_integration.hpp`**

    This file is used for formatting the fixed point number with ``PapilioFormat`` library, which is a C++20 format library.
    If you want to use it, you need to include the ``PapilioFormat`` library in your project.
    If you don't need it, you can ignore it.
B. Build and Install with XMake
-------------------------------

The library is built with `xmake`, you can use the following command to build and install the library.

.. code-block:: sh
    xmake f -c
    xmake
    xmake install
