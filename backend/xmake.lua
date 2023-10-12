add_rules("mode.debug", "mode.release")

-- Add local repository
add_repositories("repository repository")

set_languages("cxx17")

add_requires("spdlog")
add_requires("fmt")

add_requires("nlohmann_json")

-- C++ Backend API for Svelte App
add_requires("oatpp", "oatpp-websocket")

-- For Functional Programming?
add_requires("tl_expected")

target("backend")
    set_kind("binary")

    add_packages("fmt")
    add_packages("spdlog")

    add_packages("oatpp", "oatpp-websocket")

    add_packages("tl_expected")

    add_files("src/**.cpp")
    add_headerfiles("src/**.h", "src/**.hpp")
    add_includedirs("include")

    if is_plat("windows") then
        add_defines("_WIN32")
    end
