Supported Platforms
===================

- xmake >= v2.2.2
- Any C++ compiler that supports C++20

Currently, the following platforms and compilers are officially tested.

.. note::
    The 64-bit fixed point number uses ``eirin::detail::int128_t`` as the intermediate type. Currently, it depends on the compiler extension, and for the compiler that does not support it, the 64-bit fixed point number will not be available.
    If you need the 64-bit fixed point number, please use the compiler that supports it or some third-party library to provide the 128-bit integer type (and typedef the fixed64 yourself).
    In MSVC, the int128_t is typedefed as ``std::_Signed128`` and ``std::_Unsigned128`` in ``__msvc_int128.hpp`` header file.
    And in Linux Clang/GCC, it is typedefed as ``__int128_t`` and ``unsigned __int128_t`` in ``__int128_t.h`` header file.

Integrate into Your Project
===========================

A. Copy into Your Project
-------------------------

The core library of EirinFixed is a header-only library, you can copy the ``include`` directory into your project, and include the header file in your code.

To be noticed that the header file ``parse.hpp`` is not a header-only library, and it includes the interface used for binding fixed point numbers to AngelScript for my another project. If you don't need it, you can ignore it.

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
