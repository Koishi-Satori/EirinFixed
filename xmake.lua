set_project("eirin_fixed")
set_version("1.0.2")

add_rules("mode.debug", "mode.release")

add_repositories("eirin_repo eirin_repo")

option("eirin_with_papilio")
    set_default(false)
    set_showmenu(true)
    set_description("Enable papilio support")
    add_defines("EIRIN_WITH_PAPILIO")
    option_end()
option("eirin_build_tests")
    set_default(true)
    set_showmenu(true)
    set_description("Build unit tests")
    add_defines("EIRIN_BUILD_TEST")
    option_end()
option("eirin_build_benchmarks")
    set_default(true)
    set_showmenu(true)
    set_description("Build fixed number and double benchmarks")
    add_defines("EIRIN_BUILD_BENCHMARK")
    option_end()

add_requires("boost 1.86.0", {configs = {multiprecision = true, container = true, math = true, cmake = false}})
if has_config("eirin_with_papilio") then
    add_requires("papilio")
end

set_warnings("all")
set_languages("cxx20")

target("eirin_fixed")
    add_options("eirin_with_papilio")
    set_kind("static")
    add_includedirs("include", {public = true})
    add_files("src/*.cpp")
    if has_config("eirin_with_papilio") then
        add_packages("papilio")
    end
    add_packages("boost", {public = true})
    target_end()

if has_config("eirin_build_tests") then
    includes("test")
end
if has_config("eirin_build_benchmarks") then
    includes("benchmark")
end
