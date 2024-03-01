add_repositories("local-repo repository")

includes("common/xmake.lua", "tui/xmake.lua")

if is_plat("linux") then 
    includes("daemon/xmake.lua")
end
