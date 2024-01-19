add_rules("mode.debug", "mode.release")

-- $ sudo apt-get install python3 python3-pip python3-setuptools \
                    --    python3-wheel ninja-build
-- sudo apt-get install libglib2.0-dev

add_requires("cartographer") -- Install `sudo apt-get install gfortran`
add_requires("cpp-dump")

target("lidar")
    set_kind("binary")
    set_languages("cxx17")

    add_packages("cartographer")
    add_packages("cpp-dump")

    add_files("src/*.cpp")

    -- add_defines("NOMINMAX")
    -- add_defines("GLOG_NO_ABBREVIATED_SEVERITIES")
