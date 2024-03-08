add_rules("mode.debug", "mode.release")

add_requires("ftxui")

includes("../common/xmake.lua")

target("rpi_tui")
    set_kind("binary")
    
    set_languages("cxx17")
    
    add_packages("ftxui")

    add_headerfiles("src/**.h")
    add_files("src/**.cpp", "src/**.cxx")

    add_deps("rpi_common", {public = true})

    set_configdir("$(buildir)/$(plat)/$(arch)/$(mode)")
    add_configfiles("settings/config.jsonc", {onlycopy = true, prefixdir = "settings"})

    on_install(function (target) end)
    on_uninstall(function (target) end)
