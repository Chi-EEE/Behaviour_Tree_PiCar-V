add_rules("mode.debug", "mode.release")

includes("../utils/xmake.lua")
includes("../behaviour_tree/xmake.lua")

add_requires("spdlog")

target("behaviour_tree_car")
    set_kind("headeronly")
    set_languages("cxx17")
    set_group("other")

    add_deps("utils", {public = true})
    add_deps("behaviour_tree", {public = true})

    add_packages("spdlog", {public = true})

    add_includedirs("include", {public = true})
    add_headerfiles("include/(**.hpp)")

    add_defines("NOMINMAX")

    on_install(function (target) end)
    on_uninstall(function (target) end)
