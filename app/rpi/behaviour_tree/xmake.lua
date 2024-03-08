add_rules("mode.debug", "mode.release")

add_requires("cxxopts")

includes("../common/xmake.lua")

target("rpi_behaviour_tree")
    set_kind("binary")
    
    set_languages("cxx17")

    add_packages("cxxopts")
    
    add_headerfiles("src/**.h")
    add_files("src/**.cpp")

    add_deps("rpi_common", {public = true})

    set_configdir("$(buildir)/$(plat)/$(arch)/$(mode)")
    add_configfiles("settings/config.jsonc", {onlycopy = true, prefixdir = "settings"})

    on_install(function (target) end)
    on_uninstall(function (target) end)
