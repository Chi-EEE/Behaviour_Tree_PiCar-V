package("wiringpi")
    set_homepage("https://github.com/WiringPi/WiringPi")
    set_description("Gordon's Arduino wiring-like WiringPi Library for the Raspberry Pi (Unofficial Mirror for WiringPi bindings)")
    set_license("LGPL-3.0")

    set_urls("https://github.com/WiringPi/WiringPi.git")

    add_versions("2022.02.09", "f97a6230160b819e6daea7eb242404afa708e421")

    on_install("linux", "macosx", function (package)
        os.cd("wiringPi")
        io.writefile("xmake.lua", [[
            add_rules("mode.debug", "mode.release")
            target("wiringpi")
                set_kind("$(kind)")
                set_languages("c++11")
                add_files("*.c")
                add_headerfiles("*.h")
                add_includedirs(".")
        ]])
        local configs = {}
        import("package.tools.xmake").install(package, configs)
    end)

    on_test(function (package)
        assert(package:check_cxxsnippets({test = [[
            #include <wiringPi.h>
            void test() {
                wiringPiSetup();
            }
        ]]}, {configs = {languages = "c++11"}}))
    end)
