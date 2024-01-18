add_rules("mode.debug", "mode.release")

add_requires("fmt")
add_requires("tl_expected")

target("behaviour_tree")
    set_kind("$(kind)")
    set_languages("cxx17")
    add_packages("fmt")
    add_packages("tl_expected")

    add_includedirs("include", {public = true})
    add_headerfiles("include/(**.hpp)")
    add_files("include/**.cpp")
