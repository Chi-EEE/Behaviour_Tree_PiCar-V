add_rules("mode.debug", "mode.release")

-- Add local repository
add_repositories("repository repository")

set_languages("cxx17")

-- Logging / Data handling
add_requires("boost", { configs = {chrono = true} })
add_requires("fmt", "spdlog")
add_requires("nlohmann_json")

-- C++ Backend API for Svelte App
add_requires("oatpp", "oatpp-websocket")

-- For Functional Programming?
add_requires("tl_expected")

target("backend")
    set_kind("binary")

    add_packages("boost")
    add_packages("fmt", "spdlog")
    add_packages("nlohmann_json")
    add_packages("oatpp", "oatpp-websocket")
    add_packages("tl_expected")

    before_build_files(function(target)
        local frontend_dir = path.join(os.scriptdir(), "..", "frontend")
        os.execv("pnpm", {"--prefix", frontend_dir, "i"})
        os.execv("pnpm", {"--prefix", frontend_dir, "run", "build"})
        print("Copying svelte files...")
        os.cp("$(scriptdir)/../frontend/public", "$(buildir)/$(plat)/$(arch)/$(mode)/frontend/public")
        print("Completed copying svelte files...")
    end)

    add_files("src/**.cpp")
    add_headerfiles("src/**.hpp")
    add_files("src/controller/**.cc")

    add_includedirs("include")

    if is_plat("windows") then
        add_defines("_WIN32")
    end

     set_configdir("$(buildir)/$(plat)/$(arch)/$(mode)/config")
     add_configfiles("config/*.json", {onlycopy = true})

    
for _, file in ipairs(os.files("tests/test_*.cpp")) do
    local name = path.basename(file)
    target(name)
        set_kind("binary")
        set_default(false)
        add_packages("boost")
        add_packages("fmt", "spdlog")
        add_packages("nlohmann_json")
        add_packages("oatpp", "oatpp-websocket")
        add_packages("tl_expected")
        add_files("tests/" .. name .. ".cpp")
        add_files("src/controller/**.cc")
        -- add_tests("default")
        -- add_tests("args", {runargs = {"foo", "bar"}})
        -- add_tests("pass_output", {trim_output = true, runargs = "foo", pass_outputs = "hello foo"})
        -- add_tests("fail_output", {fail_outputs = {"hello2 .*", "hello xmake"}})
end