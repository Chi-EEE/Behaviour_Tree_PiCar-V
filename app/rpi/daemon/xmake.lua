set_project("rpi_daemon")

local description = [[Raspberry Pi Daemon]]
set_description(description)

set_version("1")

add_rules("mode.debug", "mode.release")

set_allowedplats("linux")

add_requires("cpptrace")
add_requires("daemonpp-fork", {system = false})

includes("../common/xmake.lua")

target("rpi_daemon")
    set_kind("binary")

    set_languages("cxx17")
    
    add_packages("cpptrace")
    add_packages("daemonpp-fork")

    add_headerfiles("src/**.h")
    add_files("src/**.cpp")

    add_deps("rpi_common", {public = true})

    add_defines("__linux")
    
    local dir = "$(buildir)/$(plat)/$(arch)/$(mode)"
    set_configdir(dir)
    add_configfiles("(systemd/**)", { onlycopy = true })
    add_configfiles("install/(**)", { onlycopy = true })

    local conf_install_dir = "etc/rpi_daemon"
    local install_files = {
        {"rpi_daemon", "usr/bin"},
        {"systemd/rpi_daemon.service","etc/systemd/system"},
        {"systemd/rpi_daemon.conf",conf_install_dir}
    }

    on_install(function (target)
        for _, file_pair in ipairs(install_files) do
            os.cp(dir .. "/" .. file_pair[1], "/" .. file_pair[2] .. "/")
            print("installing " .. file_pair[1] .. " to /" .. file_pair[2] .. " ..")
        end
    end)

    on_uninstall(function (target)
        for _, file_pair in ipairs(install_files) do
            os.rm("/" .. file_pair[2] .. "/" .. path.filename(file_pair[1]))
            print("uninstalling " .. path.filename(file_pair[1]) .. " from /" .. file_pair[2] .. " ..")
        end
        os.rmdir("/" .. conf_install_dir)
        print("removing directory /" .. conf_install_dir .. " ..") 
    end)

    after_build(function (target) 
        io.replace(dir .. "/systemd/rpi_daemon.conf", "@PROJECT_NAME@", target:name(), {plain = true})
        io.replace(dir .. "/systemd/rpi_daemon.conf", "@PROJECT_VERSION@", target:version(), {plain = true})
        io.replace(dir .. "/systemd/rpi_daemon.conf", "@PROJECT_DESCRIPTION@", description, {plain = true})

        io.replace(dir .. "/systemd/rpi_daemon.service", "@PROJECT_NAME@", target:name(), {plain = true})
        io.replace(dir .. "/systemd/rpi_daemon.service", "@PROJECT_VERSION@", target:version(), {plain = true})
        io.replace(dir .. "/systemd/rpi_daemon.service", "@PROJECT_DESCRIPTION@", description, {plain = true})
    end)
    