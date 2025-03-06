#include <benchmark/benchmark.h>
#include "bench.hpp"

static void double_create(benchmark::State& state)
{
    for(auto _ : state)
    {
        auto fp1 = db_identity(1145.14);
    }
}

static void double_divide(benchmark::State& state)
{
    auto d2 = 4.95;
    for(auto _ : state)
    {
        auto d1 = 1145.14;
        db_identity(d1 /= d2);
    }
}

static void double_multiple(benchmark::State& state)
{
    auto d2 = 4.95;
    for(auto _ : state)
    {
        auto d1 = 1145.14;
        db_identity(d1 *= d2);
    }
}

static void double_add(benchmark::State& state)
{
    auto d2 = 4.95;
    for(auto _ : state)
    {
        auto d1 = 1145.14;
        db_identity(d1 += d2);
    }
}

static void double_minus(benchmark::State& state)
{
    auto d2 = 4.95;
    for(auto _ : state)
    {
        auto d1 = 1145.14;
        db_identity(d1 -= d2);
    }
}

static void double_sqrt(benchmark::State& state)
{
    for(auto _ : state)
    {
        auto d1 = 1145.14;
        db_identity(sqrt(d1));
    }
}

static void double_log2(benchmark::State& state)
{
    for(auto _ : state)
    {
        auto d1 = 1145.14;
        db_identity(log2(d1));
    }
}

static void double_log(benchmark::State& state)
{
    for(auto _ : state)
    {
        auto d1 = 1145.14;
        db_identity(log(d1));
    }
}

static void double_log10(benchmark::State& state)
{
    for(auto _ : state)
    {
        auto d1 = 1145.14;
        db_identity(log(d1));
    }
}

static void double_exp(benchmark::State& state)
{
    for(auto _ : state)
    {
        auto d1 = 1145.14;
        db_identity(exp(d1));
    }
}

static void double_pow(benchmark::State& state)
{
    for(auto _ : state)
    {
        auto d1 = 1145.14;
        db_identity(pow(d1, 2));
    }
}

static void double_sin(benchmark::State& state)
{
    for(auto _ : state)
    {
        auto d1 = 1145.14;
        db_identity(sin(d1));
    }
}

static void double_cos(benchmark::State& state)
{
    for(auto _ : state)
    {
        auto d1 = 1145.14;
        db_identity(cos(d1));
    }
}

static void double_tan(benchmark::State& state)
{
    for(auto _ : state)
    {
        auto d1 = 1145.14;
        db_identity(atan(d1));
    }
}

static void double_asin(benchmark::State& state)
{
    for(auto _ : state)
    {
        auto d1 = 1145.14;
        db_identity(asin(d1));
    }
}

static void double_acos(benchmark::State& state)
{
    for(auto _ : state)
    {
        auto d1 = 1145.14;
        db_identity(acos(d1));
    }
}

static void double_atan(benchmark::State& state)
{
    for(auto _ : state)
    {
        auto d1 = 1145.14;
        db_identity(atan(d1));
    }
}

BENCHMARK(double_create);
BENCHMARK(double_divide);
BENCHMARK(double_multiple);
BENCHMARK(double_add);
BENCHMARK(double_minus);
BENCHMARK(double_sqrt);
BENCHMARK(double_log2);
BENCHMARK(double_log);
BENCHMARK(double_log10);
BENCHMARK(double_exp);
BENCHMARK(double_pow);
BENCHMARK(double_sin);
BENCHMARK(double_cos);
BENCHMARK(double_tan);
BENCHMARK(double_atan);
BENCHMARK(double_acos);
BENCHMARK(double_asin);

BENCHMARK_MAIN();
