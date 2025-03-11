add_requires("gtest")

target("fixed32.test")
    set_kind("binary")
    add_files("*.cpp")
    add_deps("fixed32")
    add_packages("gtest")
