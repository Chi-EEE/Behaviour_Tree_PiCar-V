add_rules("mode.debug", "mode.release")

set_languages("cxx17")

add_repositories("local-repo repository")

-- For the Lidar Scanner
add_requires("serial")

-- Websocket Messaging
add_requires("ixwebsocket_", {configs = {use_tls = true, ssl = "mbedtls"}})
-- add_requires("easywsclient")

add_requires("nlohmann_json")

add_requires("spdlog")

add_requires("fmt")

add_requires("imath")
add_requires("effolkronium-random")

if is_plat("windows") then
    add_requires("sfml")
end

if is_plat("linux", "macosx") then
    -- For the SunFounder Car
    add_requires("pca9685")
    add_requires("wiringpi")
end

-- For Functional Programming?
add_requires("tl_expected")

target("lidar")
    set_kind("binary")
    
    add_packages("serial")
    add_packages("spdlog")
    add_packages("fmt")
    add_packages("imath")
    add_packages("effolkronium-random")

    add_packages("ixwebsocket_")
    -- add_packages("easywsclient")

    add_packages("nlohmann_json")
    
    if is_plat("windows") then
        add_packages("sfml")
    end

    add_packages("tl_expected")

    add_files("src/*.cpp")
    add_files("include/*.cpp")
    add_headerfiles("src/*.h", "src/*.hpp")
    add_includedirs("include")

    if is_plat("windows") then
        add_defines("_WIN32")
    end
