package("rplidar")
    add_deps("serial", "spdlog")
    set_sourcedir(path.join(os.scriptdir(), "rplidar"))
    set_policy("package.install_always", true)
    on_install(function (package)
        local configs = {}
        import("package.tools.xmake").install(package, configs)
    end)
package_end()