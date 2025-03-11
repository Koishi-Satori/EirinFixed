# EirinFixed

- Original: [en-us](README.md)

一个灵活且高精度的C++定点数库，提供了定点数模板类、高精度数学运算和基本都输入输出函数。可以运行```fixed.benchmark```和```double.benchmark```这两个基准测试来测试定点数类型与C++ double类型之间的性能差异，或者查看在benchmark目录下[-O3](benchmark/fixed_benchmark_O3.txt)与[-O2](benchmark/fixed_benchmark_O2.txt)优化下的运行结果，测试环境为AMD R7-7735H。

同时提供了已定义的32位宽度、16位精度定点数(```fixed32```)和64位宽度、32位精度定点数(```fixed64```)
