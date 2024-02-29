set_project("rpi_daemon")

local description = [[Raspberry Pi Daemon]]
set_description(description)

set_version("1")

add_rules("mode.debug", "mode.release")

set_allowedplats("linux")

add_requires("ixwebsocket", {configs = {use_tls = true, ssl = "mbedtls"}})
add_requires("cxxopts", "fmt", "rapidjson", "robin-map", "spdlog", "tl_expected", "nod")
add_requires("serial")
add_requires("effolkronium-random", "pugixml")

if is_plat("linux", "macosx") then
    add_requires("pca9685", {configs = {shared = true}})
    add_requires("tb6612")
end

add_requires("daemonpp-fork")

includes("../common/xmake.lua")

target("rpi_daemon")
    set_kind("binary")

    set_languages("cxx17")
    
    add_packages("ixwebsocket")
    add_packages("cxxopts", "fmt", "rapidjson", "robin-map", "spdlog", "tl_expected", "nod")
    add_packages("serial")
    add_packages("effolkronium-random", "pugixml")
    add_packages("daemonpp-fork")

    add_headerfiles("src/**.h")
    add_files("src/**.cpp")

    add_deps("rpi_common", {public = true})
    
    local dir = "$(buildir)/$(plat)/$(arch)/$(mode)"
    set_configdir(dir)
    add_configfiles("(systemd/**)", { onlycopy = true })

    after_build(function (target) 
        io.replace(dir .. "/systemd/rpi_daemon.conf", "@PROJECT_NAME@", target:name(), {plain = true})
        io.replace(dir .. "/systemd/rpi_daemon.conf", "@PROJECT_VERSION@", target:version(), {plain = true})
        io.replace(dir .. "/systemd/rpi_daemon.conf", "@PROJECT_DESCRIPTION@", description, {plain = true})

        io.replace(dir .. "/systemd/rpi_daemon.service", "@PROJECT_NAME@", target:name(), {plain = true})
        io.replace(dir .. "/systemd/rpi_daemon.service", "@PROJECT_VERSION@", target:version(), {plain = true})
        io.replace(dir .. "/systemd/rpi_daemon.service", "@PROJECT_DESCRIPTION@", description, {plain = true})

        os.cp(target:targetfile(), "/usr/bin/")
        os.cp(dir .. "/systemd/rpi_daemon.service", "/etc/systemd/system/")
        os.cp(dir .. "/systemd/rpi_daemon.conf", "/etc/systemd/system/")
    end)
    