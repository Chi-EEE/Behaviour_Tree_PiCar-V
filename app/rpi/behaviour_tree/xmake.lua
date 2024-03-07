add_rules("mode.debug", "mode.release")

add_requires("ixwebsocket", {configs = {use_tls = true, ssl = "mbedtls"}})
add_requires("fmt", "rapidjson", "robin-map", "spdlog", "tl_expected", "nod")
add_requires("serial")
add_requires("effolkronium-random", "pugixml")
add_requires("ftxui")
add_requires("cxxopts")

if is_plat("linux", "macosx") then
    add_requires("pca9685")
    add_requires("tb6612")
end

includes("../common/xmake.lua")

target("rpi_behaviour_tree")
    set_kind("binary")
    
    set_languages("cxx17")
    
    add_packages("ixwebsocket")
    add_packages("fmt", "rapidjson", "robin-map", "spdlog", "tl_expected", "nod")
    add_packages("serial", "pca9685", "tb6612")
    add_packages("effolkronium-random", "pugixml")
    add_packages("ftxui")
    add_packages("cxxopts")

    add_headerfiles("src/**.h")
    add_files("src/**.cpp", "src/**.cxx")

    add_deps("rpi_common", {public = true})

    set_configdir("$(buildir)/$(plat)/$(arch)/$(mode)")
    add_configfiles("settings/config.jsonc", {onlycopy = true, prefixdir = "settings"})

    on_install(function (target) end)
    on_uninstall(function (target) end)

    
    for _, file in ipairs(os.files("test/test_*.xml")) do
        local name = path.basename(file)
        target(name)
            set_kind("phony")
            on_build(function()
                local content = io.readfile(file)
                print(file)
                print(content)
                print(content:match("TEST ?| ?({.*})"))
            end)
    end