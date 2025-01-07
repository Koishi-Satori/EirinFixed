#include <fixed.hpp>
#include <fpmath.hpp>
#include <benchmark/benchmark.h>

static void f32_create(benchmark::State& state)
{
    for(auto _ : state)
    {
        auto fp1 = "1145.14"_f32;
    }
}

static void f32_divide(benchmark::State& state)
{
    auto fp2 = "4.95"_f32;
    for(auto _ : state)
    {
        auto fp1 = "1145.14"_f32;
        fp1 /= fp2;
    }
}

static void f32_multiple(benchmark::State& state)
{
    auto fp2 = "4.95"_f32;
    for(auto _ : state)
    {
        auto fp1 = "1145.14"_f32;
        fp1 *= fp2;
    }
}

static void f32_add(benchmark::State& state)
{
    auto fp2 = "4.95"_f32;
    for(auto _ : state)
    {
        auto fp1 = "1145.14"_f32;
        fp1 += fp2;
    }
}

static void f32_minus(benchmark::State& state)
{
    auto fp2 = "4.95"_f32;
    for(auto _ : state)
    {
        auto fp1 = "1145.14"_f32;
        fp1 -= fp2;
    }
}

static void f32_sqrt(benchmark::State& state)
{
    for(auto _ : state)
    {
        auto fp1 = "1145.14"_f32;
        sqrt(fp1);
    }
}

static void f32_log2(benchmark::State& state)
{
    for(auto _ : state)
    {
        auto fp1 = "1145.14"_f32;
        log2(fp1);
    }
}
static void f32_log(benchmark::State& state)
{
    for(auto _ : state)
    {
        auto fp1 = "1145.14"_f32;
        log(fp1);
    }
}
static void f32_log10(benchmark::State& state)
{
    for(auto _ : state)
    {
        auto fp1 = "1145.14"_f32;
        log10(fp1);
    }
}

static void f32_exp(benchmark::State& state)
{
    for(auto _ : state)
    {
        auto fp1 = "11.4514"_f32;
        exp(fp1);
    }
}
static void f32_pow(benchmark::State& state)
{
    auto fp2 = "3.5"_f32;
    for(auto _ : state)
    {
        auto fp1 = "11.4514"_f32;
        pow(fp1, fp2);
    }
}

static void f32_sin(benchmark::State& state)
{
    for(auto _ : state)
    {
        auto fp1 = "1145.14"_f32;
        sin(fp1);
    }
}
static void f32_cos(benchmark::State& state)
{
    for(auto _ : state)
    {
        auto fp1 = "1145.14"_f32;
        sin(fp1);
    }
}
static void f32_tan(benchmark::State& state)
{
    for(auto _ : state)
    {
        auto fp1 = "1145.14"_f32;
        sin(fp1);
    }
}

static void f32_atan(benchmark::State& state)
{
    for(auto _ : state)
    {
        auto fp1 = "1145.14"_f32;
        atan(fp1);
    }
}
static void f32_asin(benchmark::State& state)
{
    for(auto _ : state)
    {
        auto fp1 = "0.5"_f32;
        atan(fp1);
    }
}
static void f32_acos(benchmark::State& state)
{
    for(auto _ : state)
    {
        auto fp1 = "0.5"_f32;
        atan(fp1);
    }
}

BENCHMARK(f32_create);
BENCHMARK(f32_divide);
BENCHMARK(f32_multiple);
BENCHMARK(f32_add);
BENCHMARK(f32_minus);
BENCHMARK(f32_sqrt);
BENCHMARK(f32_log2);
BENCHMARK(f32_log);
BENCHMARK(f32_log10);
BENCHMARK(f32_exp);
BENCHMARK(f32_pow);
BENCHMARK(f32_sin);
BENCHMARK(f32_cos);
BENCHMARK(f32_tan);
BENCHMARK(f32_atan);
BENCHMARK(f32_acos);
BENCHMARK(f32_asin);

BENCHMARK_MAIN();
