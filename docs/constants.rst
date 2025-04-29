Predefined Constants
======================

There exists some pre-defined constants in the ``fixed_num`` class, and the names of these functions are listed below.

- epsilon: the epsilon of current fixed number.
- e: natural constant.
- pi: π.
- pi_2: π/2.

Numbers Header File
=====================

The header file ``eirin/numbers.hpp`` contains the constants which same as the standard library ``<numbers>`` header file.

- They are defined in the ``numbers`` namespace.
- To be noticed that, the accuracy of these constants are based on 64-bit fixed point numbers.
- And if you want more accuracy for your custom fixed number like ``fixed128``, you can check and use the auxiliary functions in ``eirin/detail/util.hpp``.
- The calculation method has been illustrated in the ``eirin/detail/util.hpp`` file.

.. list-table::
   :header-rows: 1
   :widths: auto

   * - Name
     - Description
   * - ``e_v``
     - The mathematical constant e.
   * - ``pi_v``
     - The mathematical constant pi.
   * - ``log2e_v``
     - The logarithm of e to the base 2.
   * - ``log10e_v``
     - The logarithm of e to the base 10.
   * - ``inv_pi_v``
     - The multiplicative inverse of pi.
   * - ``inv_sqrtpi_v``
     - The multiplicative inverse of the square root of pi.
   * - ``ln2_v``
     - The natural logarithm of 2.
   * - ``ln10_v``
     - The natural logarithm of 10.
   * - ``sqrt2_v``
     - The square root of 2.
   * - ``sqrt3_v``
     - The square root of 3.
   * - ``inv_sqrt3_v``
     - The multiplicative inverse of the square root of 3.
   * - ``egamma_v``
     - The Euler-Mascheroni constant.
   * - ``phi_v``
     - The golden ratio.
