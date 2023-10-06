add_rules("mode.debug", "mode.release")

target("rplidar_sdk")
    set_kind("$(kind)")
    set_languages("c++11")

    add_files(
        "src/*.cpp",
        "src/hal/*.cpp"
    )
    add_headerfiles(
        "include/*.h",
        "src/(hal/*.h)"
    )
    add_includedirs("include", {public = true})
    add_includedirs("src", {public = true})

    if is_plat("windows", "mingw") then
        if is_arch("x64") then
            add_defines("WIN64")
        elseif is_arch("x86") then
            add_defines("WIN32")
        end
        add_defines("_WIN32")
        add_files("src/arch/win32/*.cpp")
        add_headerfiles("src/win32/*.h")
    elseif is_plat("linux") then
        add_defines("__GNUC__")
        add_files("src/arch/linux/*.cpp")
        add_headerfiles("src/linux/*.h")
    elseif is_plat("macosx") then
        add_defines("_MACOS")
        add_files("src/arch/macOS/*.cpp")
        add_headerfiles("src/macOS/*.h")
    end
