add_requires("gtest")

add_requires("papilio")
target("eirin_fixed.test")
    set_warnings("all", "error")
    set_kind("binary")
    if is_plat("wasm") then
        set_extension(".js")
        set_exceptions("no-cxx")
    end
    add_tests("test_eirin_fixed")
    add_files("main.cpp")
    add_deps("eirin_fixed")
    add_packages("gtest")
    add_packages("papilio")
    if has_config("eirin_dev_test") and get_config("eirin_dev_test") == true then
        add_defines("EIRIN_DEV_TEST_MODE")
        add_cxxflags("-mavx", "-mavx2", "-msse")
    end
