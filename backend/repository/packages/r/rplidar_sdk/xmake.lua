package("rplidar_sdk")
    set_homepage("https://github.com/Slamtec/rplidar_sdk")
    set_description("Open source SDK for Slamtec RPLIDAR series products")
    set_license("BSD-2-Clause")

    set_urls("https://github.com/Slamtec/rplidar_sdk.git")

    add_versions("v1.12.0", "b9ab2c8e55c6457bc704a9269fbc4468efca156a")

    on_install(function (package)
        os.cd("sdk")
        os.cp(path.join(package:scriptdir(), "port", "xmake.lua"), "xmake.lua")
        local configs = {}
        import("package.tools.xmake").install(package, configs)
    end)

    on_test(function (package)
        assert(package:has_cfuncs("createSerialPortChannel", {includes = "sl_lidar_driver.h"}))
    end)
