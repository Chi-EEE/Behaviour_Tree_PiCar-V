package("rplidar_sdk")
    set_homepage("https://github.com/Slamtec/rplidar_sdk")
    set_description("Open source SDK for Slamtec RPLIDAR series products")
    set_license("BSD-2-Clause")

    set_urls("https://github.com/Slamtec/rplidar_sdk.git")

    add_versions("v2.0.0", "b9ab2c8e55c6457bc704a9269fbc4468efca156a")

    on_load(function (package)
        if is_plat("windows", "mingw") then
            package:add("defines", "__cplusplus")
            package:add("defines", "_WIN32")
            if is_arch("x64") then
                package:add("defines", "WIN64")
            elseif is_arch("x86") then
               package:add("defines", "WIN32")
            end
        elseif is_plat("linux") then
            package:add("defines", "__GNUC__")
        elseif is_plat("macosx") then
            package:add("defines", "_MACOS")
        end
    end)

    on_install(function (package)
        os.cd("sdk")
        os.cp(path.join(package:scriptdir(), "port", "xmake.lua"), "xmake.lua")
        local configs = {}
        import("package.tools.xmake").install(package, configs)
    end)

    on_test(function (package)
        assert(package:check_cxxsnippets({test = [[
            #include <rplidar.h>
            using namespace sl;
            #define WIN64
            void test() {
                ILidarDriver * drv = *createLidarDriver();
            }
        ]]}, {configs = {languages = "c++11"}}))
    end)
