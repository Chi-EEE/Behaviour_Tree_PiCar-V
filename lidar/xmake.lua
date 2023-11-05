add_rules("mode.debug", "mode.release")

add_requires("vcpkg::cartographer", {alias = "cartographer"})

add_repositories("my-repo repository", {rootdir = path.join(os.scriptdir(), "..")})
add_requires("rplidar")

target("lidar")
    set_kind("binary")

    add_packages("cartographer")
    
    add_packages("rplidar")

    add_files("src/*.cpp")

    add_defines("NOMINMAX")
    add_defines("GLOG_NO_ABBREVIATED_SEVERITIES")
