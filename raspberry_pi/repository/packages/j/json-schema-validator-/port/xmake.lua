add_rules("mode.debug", "mode.release")

add_requires("nlohmann_json")

target("json-schema-validator")
    set_kind("$(kind)")
    set_languages("cxx17")
    add_packages("nlohmann_json")

    add_headerfiles("src/(nlohmann/json-schema.hpp)")
    add_includedirs("src", { public = true })

    add_files("src/**.cpp")
    add_headerfiles("src/*.hpp")

    if is_plat("windows") then
        add_defines("_WIN32")
    end