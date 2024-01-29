add_rules("mode.debug", "mode.release")

add_requires("fmt")
add_requires("tl_expected")
add_requires("pugixml")
add_requires("effolkronium-random")

includes("../utils/xmake.lua")

target("behaviour_tree")
    set_kind("$(kind)")
    set_languages("cxx17")
    add_deps("utils", {inherit = true})

    add_packages("fmt")
    add_packages("tl_expected")
    add_packages("pugixml")
    add_packages("effolkronium-random")

    add_includedirs("include", {public = true})
    add_headerfiles("include/(**.hpp)", "include/(**.h)")
    add_files("include/**.cpp")

    add_defines("NOMINMAX")
