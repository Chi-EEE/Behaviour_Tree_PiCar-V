add_rules("mode.debug", "mode.release")

add_requires("ixwebsocket", {configs = {use_tls = true, ssl = "mbedtls"}})
add_requires("fmt", "rapidjson", "robin-map", "spdlog", "tl_expected", "nod")
add_requires("serial")
add_requires("effolkronium-random", "pugixml")

if is_plat("linux", "macosx") then
    add_requires("pca9685")
    add_requires("tb6612")
end

includes("../../other/utils/xmake.lua")
includes("../../other/behaviour_tree/xmake.lua", "../../other/behaviour_tree_car/xmake.lua")
includes("../../other/rplidar/xmake.lua")

if is_plat("linux") then 
    add_links("pthread")
end 

target("rpi_common")
    set_kind("$(kind)")
    set_languages("cxx17")

    add_deps("utils")
    add_deps("behaviour_tree", "behaviour_tree_car")
    add_deps("rplidar")
     
    add_packages("ixwebsocket")
    add_packages("fmt", "rapidjson", "robin-map", "spdlog", "tl_expected", "nod")
    add_packages("serial", "pca9685", "tb6612")
    add_packages("effolkronium-random", "pugixml")

    add_deps("behaviour_tree")

    add_headerfiles("include/(**.hpp)", "include/(**.h)")
    add_includedirs("include", {public = true})

    add_files("src/**.cpp")
    
    if is_plat("windows") then
        add_defines("_WIN32")
    else
        add_defines("__linux")
    end

    on_install(function (target) end)
    on_uninstall(function (target) end)

target_end()
    
if is_plat("linux") then
    for _, file in ipairs(os.files("tests/pca9685/test_*.cpp")) do
        local name = path.basename(file)
        target(name)
            set_kind("binary")
            set_default(false)
            
            add_packages("pca9685")

            add_files("tests/pca9685/" .. name .. ".cpp")
        target_end()
    end
    for _, file in ipairs(os.files("tests/tb6612/test_*.cpp")) do
        local name = path.basename(file)
        target(name)
            set_kind("binary")
            set_default(false)
            
            add_packages("pca9685")
            add_packages("tb6612")

            add_files("tests/tb6612/" .. name .. ".cpp")
        target_end()
    end
end