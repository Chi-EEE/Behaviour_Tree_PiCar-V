add_rules("mode.debug", "mode.release")

add_requires("fmt")
add_requires("tl_expected")
add_requires("pugixml")

includes("../utils/xmake.lua")
includes("../behaviour_tree/xmake.lua")

import("../behaviour_tree/xmake.lua")
local behaviour_tree_packages = behaviour_tree_packages

target("behaviour_tree_car")
    set_kind("headeronly")
    set_languages("cxx17")
    set_group("other")

    add_deps("utils", {inherit = true})
    add_deps("behaviour_tree", {inherit = true})

    add_packages(behaviour_tree_packages)

    add_includedirs("include", {public = true})
    add_headerfiles("include/(**.hpp)")

    add_defines("NOMINMAX")

    on_install(function (target) end)
    on_uninstall(function (target) end)
target_end()

add_requires("cxxopts")

target("behaviour_tree_test")
    set_kind("binary")
    set_languages("cxx17")

    add_packages(behaviour_tree_packages)
    add_packages("cxxopts")

    add_deps("behaviour_tree_car")

    add_files("test/test.cpp")

    on_test(function (target, opt)
        print(target)
        print(opt.name, opt.runenvs, opt.runargs, opt.pass_outputs)
        return true
    end)
    for _, file in ipairs(os.files("test/test_*.xml")) do
        local name = path.basename(file)
        add_tests(name, {runargs = {"--test_file", path.join("$(scriptdir)", file)}})
    end

return
{
    behaviour_tree_packages = behaviour_tree_packages
}