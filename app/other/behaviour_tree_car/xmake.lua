add_rules("mode.debug", "mode.release")

add_requires("fmt")
add_requires("tl_expected")
add_requires("pugixml")

includes("../utils/xmake.lua")
includes("../behaviour_tree/xmake.lua")

target("behaviour_tree_car")
    set_kind("headeronly")
    set_languages("cxx17")

    add_deps("utils", {inherit = true})
    add_deps("behaviour_tree", {inherit = true})

    add_packages("fmt")
    add_packages("tl_expected")
    add_packages("pugixml")

    add_includedirs("include", {public = true})
    add_headerfiles("include/(**.hpp)")
