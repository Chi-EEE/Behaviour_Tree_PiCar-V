add_rules("mode.debug", "mode.release")


-- Logging / Data handling
add_requires("boost", { configs = {chrono = true} })
add_requires("fmt", "spdlog")
add_requires("nlohmann_json", "rapidjson", "pugixml v1.14")

add_requires("robin-map")

-- C++ Backend API for Svelte App
add_requires("drogon", { configs = {sqlite3 = true, mysql = true}})

-- For Rust-Like Programming
add_requires("tl_expected")

includes("../other/utils/xmake.lua")
includes("../other/behaviour_tree/xmake.lua", "../other/behaviour_tree_car/xmake.lua")

target("backend")
    set_kind("binary")
    set_languages("cxx17")

    add_deps("utils")
    add_deps("behaviour_tree", "behaviour_tree_car")

    add_defines("BEHAVIOUR_TREE_DISABLE_RUN")

    add_packages("boost")
    add_packages("fmt", "spdlog")
    add_packages("nlohmann_json", "rapidjson", "pugixml")
    add_packages("robin-map")
    add_packages("drogon")

    add_packages("tl_expected")

    add_headerfiles("src/**.hpp")
    add_headerfiles("src/**.h")
    add_files("src/**.cpp")

    if is_plat("windows") then
        add_defines("_WIN32")
    end

    -- Could improve this so that it only builds the frontend if it has changed
    after_build_files(function(target)
        import("core.project.config")
        import("core.base.json")

        config.load()

        local exe_dir = path.join(os.projectdir(), config.get("buildir"), config.get("plat"), config.get("arch"), config.get("mode"))

        local static = path.join(exe_dir, "public")
        if os.exists(static) then
            os.rmdir(static)
        end

        local config_table = json.loadfile(path.join(os.scriptdir(), "settings", "config.json"))
        config_table["app"]["document_root"] = static
        
        local config_json = json.encode(config_table)

        io.writefile(path.join(exe_dir, "settings", "config.json"), config_json)

        local frontend_dir = path.join(path.directory(os.scriptdir()), "frontend")
        os.execv("pnpm", {"--prefix", frontend_dir, "i"})
        os.execv("pnpm", {"--prefix", frontend_dir, "run", "build"})

        print("Copying svelte files...")
        os.cp(path.join(path.directory(os.scriptdir()), "frontend", "dist"), static)
        print("Completed copying svelte files...")
    end)

-- From xmake sample code:
-- for _, file in ipairs(os.files("tests/test_*.cpp")) do
--     local name = path.basename(file)
--     target(name)
--         set_kind("binary")
--         set_default(false)
--         add_packages("boost")
--         add_packages("fmt", "spdlog")
--         add_packages("nlohmann_json")
--         add_packages("oatpp", "oatpp-websocket")
--         add_packages("tl_expected")
--         add_files("tests/" .. name .. ".cpp")
--         add_files("src/controller/**.cc")
--         -- add_tests("default")
--         -- add_tests("args", {runargs = {"foo", "bar"}})
--         -- add_tests("pass_output", {trim_output = true, runargs = "foo", pass_outputs = "hello foo"})
--         -- add_tests("fail_output", {fail_outputs = {"hello2 .*", "hello xmake"}})
-- end