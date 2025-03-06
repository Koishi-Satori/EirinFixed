add_requires("benchmark")

target("fixed32.benchmark")
    set_kind("binary")
    add_includedirs(".", {public = true})
    add_files("./*.cpp")
    add_deps("fixed32")
    add_packages("benchmark")
