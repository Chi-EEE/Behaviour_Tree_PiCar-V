target("utils")
    set_kind("headeronly")
    set_group("other")
    
    add_includedirs("include", {public = true})
    add_headerfiles("include/(**.hpp)")

    on_install(function (target) end)
    on_uninstall(function (target) end)
    