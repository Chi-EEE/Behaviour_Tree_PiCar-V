add_rules("mode.debug", "mode.release")

-- Add local repository
add_repositories("repository repository")

set_languages("cxx17")

-- For the Lidar Scanner
add_requires("serial")

add_requires("spdlog")

add_requires("fmt")

-- C++ Backend API for Svelte App
add_requires("oatpp")

if is_plat("windows") then
    add_requires("sfml")
end

if is_plat("linux", "macosx") then
    -- For the SunFounder Car
    add_requires("pca9685")
end

-- For Functional Programming?
add_requires("tl_expected")

target("backend")
    set_kind("binary")
    add_packages("serial")
    add_packages("spdlog")
    add_packages("fmt")

    add_packages("oatpp")
    
    if is_plat("windows") then
        add_packages("sfml")
    end
    if is_plat("linux", "macosx") then
        add_packages("pca9685")
    end

    add_packages("tl_expected")

    add_files("src/*.cpp")
    add_headerfiles("src/*.h", "src/*.hpp")

    if is_plat("windows") then
        add_defines("_WIN32")
    end
