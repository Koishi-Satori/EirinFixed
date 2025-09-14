set_project("eirin_fixed")
set_version("1.0.2")

add_rules("mode.debug", "mode.release")

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

if has_config("eirin_build_tests") then
    -- Add dependency for tests --
    add_repositories("FRIKEE git@github.com:FRI-KEE/FRIKEE-xrepo.git")
end

set_warnings("all")
set_languages("cxx20")

target("eirin_fixed")
    set_kind("static")
    add_includedirs("include", {public = true})
    add_headerfiles("include/(eirin/**.hpp)")
    add_files("src/*.cpp")
    if is_mode("release") then
        set_optimize("fastest")
    end
    target_end()

if has_config("eirin_build_tests") then
    includes("test")
end
if has_config("eirin_build_benchmarks") then
    includes("benchmark")
end
