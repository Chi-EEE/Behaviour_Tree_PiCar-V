add_rules("mode.debug", "mode.release")

add_requires("cxxopts", "fmt", "ixwebsocket", "spdlog", "tl_expected", "rapidjson", "nod")
add_requires("serial")
add_requires("effolkronium-random", "pugixml")
add_requires("ftxui")

includes("../raspberry_pi_common/xmake.lua")

target("raspberry_pi_tui")
    set_kind("binary")
    set_languages("cxx17")
    
    add_packages("cxxopts", "fmt", "ixwebsocket", "rapidjson", "robin-map", "spdlog", "tl_expected", "nod")
    add_packages("serial")
    add_packages("effolkronium-random", "pugixml")
    add_packages("ftxui")

    add_headerfiles("src/**.h")
    add_files("src/**.cpp", "src/**.cxx")

    add_deps("raspberry_pi_common", {public = true})