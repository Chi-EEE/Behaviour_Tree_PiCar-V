package("rplidar")
    add_deps("serial", "spdlog", "tl_expected")
    set_policy("package.install_always", true)
    on_install(function (package)
        local configs = {}
        os.cd(path.join(os.scriptdir(), "rplidar"))
        import("package.tools.xmake").install(package, configs)
    end)
package_end()