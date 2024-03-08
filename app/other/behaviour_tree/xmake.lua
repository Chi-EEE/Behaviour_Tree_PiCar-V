add_rules("mode.debug", "mode.release")

add_requires("fmt", "tl_expected", "pugixml", "effolkronium-random")

includes("../utils/xmake.lua")

target("behaviour_tree")
    set_kind("$(kind)")
    set_languages("cxx17")
    set_group("other")
   
    add_deps("utils", {public = true})

    add_packages("fmt", "tl_expected", "pugixml", "effolkronium-random", {public = true})

    add_includedirs("include", {public = true})
    add_headerfiles("include/(**.hpp)", "include/(**.h)")
    add_files("include/**.cpp")

    add_defines("NOMINMAX")

    on_install(function (target) end)
    on_uninstall(function (target) end)

return {behaviour_tree_packages = behaviour_tree_packages}