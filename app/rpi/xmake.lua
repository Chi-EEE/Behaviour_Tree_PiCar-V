set_policy("build.sanitizer.thread", true)
set_policy("build.sanitizer.memory", true)
set_policy("build.sanitizer.leak", true)
set_policy("build.sanitizer.undefined", true)

add_repositories("local-repo repository")

includes("common/xmake.lua", "tui/xmake.lua", "behaviour_tree/xmake.lua")

if is_plat("linux") then 
    includes("daemon/xmake.lua")
end
