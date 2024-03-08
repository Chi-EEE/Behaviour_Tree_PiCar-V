add_rules("mode.debug", "mode.release")

add_requires("cxxopts", "rapidjson")

includes("../behaviour_tree/xmake.lua", "../behaviour_tree_car/xmake.lua")

target("behaviour_tree_cli")
    set_kind("binary")
    set_languages("cxx17")

    add_packages("cxxopts", "rapidjson")

    add_deps("behaviour_tree", {public = true})
    add_deps("behaviour_tree_car", {public = true})

    add_defines("BEHAVIOUR_TREE_DISABLE_RUN")

    add_files("src/*.cpp")

    on_install(function (target) end)
    on_uninstall(function (target) end)
    