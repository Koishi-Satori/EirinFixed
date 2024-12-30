package("papilio")
    set_license("MIT")
    add_urls(
        "https://github.com/HenryAWE/PapilioCharontis/archive/$(version).tar.gz",
        "https://github.com/HenryAWE/PapilioCharontis.git",
        "https://gitee.com/HenryAWE/PapilioCharontis.git"
    )
    add_versions("v1.0.0", "326060bf77f28be863620b761832f6022498416dc8e2cf40c9ea77b0c015ddbc")

    add_deps("cmake")

    on_install(function (package)
        local configs = {}
        table.insert(configs, "-DCMAKE_BUILD_TYPE=" .. (package:debug() and "Debug" or "Release"))
        import("package.tools.cmake").install(package, configs)
    end)
