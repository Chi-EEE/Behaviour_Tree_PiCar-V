add_rules("mode.debug", "mode.release")

-- Add local repository
add_repositories("repository repository")

set_languages("cxx17")

-- Logging / Data handling
add_requires("boost")
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
        os.execv("pnpm", {"--prefix", path.join(os.scriptdir(), "client"), "run", "build"})
        print("Copying svelte files...")
        os.cp("$(scriptdir)/client/public", "$(buildir)/$(plat)/$(arch)/$(mode)/client/public")
        print("Completed copying svelte files...")
    end)

    add_files("src/**.cpp")
    add_headerfiles("src/**.hpp")
    add_files("src/controller/**.cc")

    add_includedirs("include")

    if is_plat("windows") then
        add_defines("_WIN32")
    end
