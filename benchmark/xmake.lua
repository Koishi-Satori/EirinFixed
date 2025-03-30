add_requires("benchmark")

target("eirin_fixed.benchmark")
    set_kind("binary")
    add_includedirs(".", {public = true})
    add_files("./benchmark.cpp", "./bench.cpp")
    add_deps("eirin_fixed")
    add_packages("benchmark")

target("double.benchmark")
    set_kind("binary")
    add_includedirs(".", {public = true})
    add_files("./double_bench.cpp", "./bench.cpp")
    add_deps("eirin_fixed")
    add_packages("benchmark")
