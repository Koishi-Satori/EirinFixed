add_requires("benchmark")

target("eirin_fixed.benchmark")
    set_kind("binary")
    add_includedirs(".", {public = true})
    add_files("./benchmark.cpp", "./bench.cpp")
    add_deps("eirin_fixed")
    add_packages("benchmark")
    -- add -Wmaybe-uninitialized on linux to catch uninitialized variable usage
    -- to be noticed that in windows/msvc this flag is not available
    if is_os("linux") then
        add_cxxflags("-Wmaybe-uninitialized", {force = true})
    end
    after_build(function(target)
        os.cp("$(scriptdir)/*.in", target:targetdir() .. "/benchmark_input/")
    end)

target("double.benchmark")
    set_kind("binary")
    add_includedirs(".", {public = true})
    add_files("./double_bench.cpp", "./bench.cpp")
    add_deps("eirin_fixed")
    add_packages("benchmark")
    -- add -Wmaybe-uninitialized on linux to catch uninitialized variable usage
    -- to be noticed that in windows/msvc this flag is not available
    if is_os("linux") then
        add_cxxflags("-Wmaybe-uninitialized", {force = true})
    end
    after_build(function(target)
        os.cp("$(scriptdir)/*.in", target:targetdir() .. "/benchmark_input/")
    end)
