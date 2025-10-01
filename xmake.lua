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
    package("papilio")
        set_license("MIT")
        add_urls(
            "https://github.com/HenryAWE/PapilioCharontis/archive/$(version).tar.gz",
            "https://github.com/HenryAWE/PapilioCharontis.git",
            "https://gitee.com/HenryAWE/PapilioCharontis.git"
        )
        add_versions("v1.0.0", "326060bf77f28be863620b761832f6022498416dc8e2cf40c9ea77b0c015ddbc")
        add_versions("v1.1.0", "23a08eedddc4369db6053955684bc7b1eac122486cacfe6a21990081950fe0c6")

        add_deps("cmake")

        on_install(function (package)
            local configs = {}
            table.insert(configs, "-DCMAKE_BUILD_TYPE=" .. (package:debug() and "Debug" or "Release"))
            import("package.tools.cmake").install(package, configs)
        end)
    package_end()
end

set_warnings("all")
set_languages("cxx20")

target("eirin_fixed")
    add_options("eirin_with_papilio")
    set_kind("headeronly")
    add_includedirs("include", {public = true})
    add_headerfiles("include/(eirin/**.hpp)")
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
