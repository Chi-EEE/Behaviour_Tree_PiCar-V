add_requires("serial", "spdlog", "tl_expected")

target("rplidar")
    set_kind("$(kind)")
    set_group("other")

    add_packages("serial", "spdlog", "tl_expected")

    add_files("src/*.cpp")
    add_headerfiles("include/(RPLidar.h)")
    add_includedirs("include", {public = true})
