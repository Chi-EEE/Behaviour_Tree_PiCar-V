add_rules("mode.debug", "mode.release")
add_requires("sockpp")

target("raspberry-pi")
    set_kind("binary")
    add_files("src/*.cpp")
    add_packages("sockpp")
