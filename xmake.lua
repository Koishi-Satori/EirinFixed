add_rules("mode.debug", "mode.release")

add_repositories("fixed32_repo fixed32_repo")

add_requires("papilio")

set_languages("cxx20")
add_cxxflags("-O3")

target("fixed32")
    set_kind("shared")
    add_includedirs("include", {public = true})
    add_files("src/*.cpp")
    add_packages("papilio", {public = true})

includes("test")
includes("benchmark")
