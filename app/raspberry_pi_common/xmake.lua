add_rules("mode.debug", "mode.release")

add_repositories("local-repo repository")

-- Messaging System
add_requires("ixwebsocket", {configs = {use_tls = true, ssl = "mbedtls"}})
add_requires("nod")

add_requires("effolkronium-random")
add_requires("robin-map")

-- Logging and JSON
add_requires("rapidjson")
add_requires("pugixml v1.14")
add_requires("spdlog")
add_requires("fmt")

-- SunFounder Car
add_requires("opencv 4.8.0")
if is_plat("linux", "macosx") then
    add_requires("pca9685", {configs = {shared = true}})
    add_requires("tb6612")
end
add_requires("serial")

-- For Better Programming
add_requires("tl_expected")

add_requires("cxxopts")

includes("../other/utils/xmake.lua")
includes("../other/behaviour_tree/xmake.lua", "../other/behaviour_tree_car/xmake.lua")
includes("../other/rplidar/xmake.lua")

if is_plat("linux") then 
    add_links("pthread")
end 

target("raspberry_pi_common")
    set_kind("$(kind)")
    set_languages("cxx17")

    add_deps("utils")
    add_deps("behaviour_tree", "behaviour_tree_car")
    add_deps("rplidar")
     
    add_packages("spdlog")
    add_packages("fmt")
    add_packages("rapidjson")
    add_packages("pugixml")

    add_packages("effolkronium-random")
    add_packages("robin-map")

    add_packages("tl_expected")
    
    add_packages("cxxopts")

    add_packages("ixwebsocket")
    add_packages("nod")

    add_packages("opencv")

    add_packages("pca9685")
    add_packages("tb6612")

    add_packages("serial")

    add_deps("behaviour_tree")

    add_headerfiles("include/(**.hpp)", "include/(**.h)")
    add_includedirs("include", {public = true})

    add_files("src/**.cpp")
    
    if is_plat("windows") then
        add_defines("_WIN32")
    else
        add_defines("__linux")
    end

    set_configdir("$(buildir)/$(plat)/$(arch)/$(mode)")
    add_configfiles("settings/config.jsonc", {onlycopy = true, prefixdir = "settings"})
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