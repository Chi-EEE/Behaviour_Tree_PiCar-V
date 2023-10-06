add_rules("mode.debug", "mode.release")

target("rplidar_sdk")
    set_kind("$(kind)")
    set_languages("cxx14")
    add_files(
        "src/*.cpp",
        "src/hal/*.cpp"
    )
    add_headerfiles(
        "include/*.h",
        "src/hal/*.h"
    )
    add_includedirs("include", {public = true})

    if is_plat("windows", "mingw") then
        add_defines("_WIN32")
        add_files("src/win32/*.cpp")
        add_headerfiles("src/win32/*.h")
    end
    if is_plat("linux") then
        add_defines("__GNUC__")
        add_files("src/linux/*.cpp")
        add_headerfiles("src/linux/*.h")
    end
    if is_plat("macosx") then
        add_defines("_MACOS")
        add_files("src/macOS/*.cpp")
        add_headerfiles("src/macOS/*.h")
    end
