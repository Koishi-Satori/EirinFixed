add_requires("gtest")

target("eirin_fixed.test")
    set_kind("binary")
    add_files("*.cpp")
    add_deps("eirin_fixed")
    add_packages("gtest")
