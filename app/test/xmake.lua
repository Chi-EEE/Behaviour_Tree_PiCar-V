includes("../other/rplidar/xmake.lua")
add_requires("rapidjson")

target("lLidar")
    set_kind("binary")
    set_languages("c++17")

    add_headerfiles("src/**.h")
    add_files("src/**.cpp")

    add_packages("rapidjson")
    add_deps("rplidar")