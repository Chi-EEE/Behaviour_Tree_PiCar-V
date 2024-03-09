add_rules("mode.debug", "mode.release")

includes("../behaviour_tree_car/xmake.lua")

add_requires("cxxopts")

target("behaviour_tree_tester")
    set_kind("binary")
    set_languages("cxx17")
    set_group("other")
    set_default(false)

    add_packages("cxxopts")

    add_deps("behaviour_tree_car", {public = true})

    add_defines("BEHAVIOUR_TREE_DISABLE_RUN")

    add_files("src/main.cpp")

    do -- [Test Cases]
        add_tests("test_hello_world", {runargs = {"--test_file", path.join(os.scriptdir(), "test/test_hello_world.xml")}, run_timeout = 1000, pass_outputs = "Hello World"})
        add_tests("test_double_nested_sequence", {runargs = {"--test_file", path.join(os.scriptdir(), "test/test_double_nested_sequence.xml")}, run_timeout = 1000, pass_outputs = "123456"})
        add_tests("test_pause_execution", {runargs = {"--test_file", path.join(os.scriptdir(), "test/test_pause_execution.xml")}, run_timeout = 2000, pass_outputs = "Succeed"})
        add_tests("test_selector", {runargs = {"--test_file", path.join(os.scriptdir(), "test/test_selector.xml")}, run_timeout = 1000, pass_outputs = "1"})
        add_tests("test_sequence", {runargs = {"--test_file", path.join(os.scriptdir(), "test/test_sequence.xml")}, run_timeout = 1000, pass_outputs = "123"})
        add_tests("test_invert_sequence", {runargs = {"--test_file", path.join(os.scriptdir(), "test/test_invert_sequence.xml")}, run_timeout = 1000, pass_outputs = "Succeed"})
        add_tests("test_invert_sequence_fail", {runargs = {"--test_file", path.join(os.scriptdir(), "test/test_invert_sequence_fail.xml")}, run_timeout = 1000, fail_outputs = "Fail"})
        add_tests("test_use_root", {runargs = {"--test_file", path.join(os.scriptdir(), "test/test_use_root.xml")}, run_timeout = 1000, pass_outputs = "Succeed"})
        add_tests("test_use_root_invert", {runargs = {"--test_file", path.join(os.scriptdir(), "test/test_use_root_invert.xml")}, run_timeout = 1000, pass_outputs = "Succeed"})
        add_tests("test_use_root_invert_fail", {runargs = {"--test_file", path.join(os.scriptdir(), "test/test_use_root_invert_fail.xml")}, run_timeout = 1000, fail_outputs = "Fail"})
        add_tests("test_random", {runargs = {"--test_file", path.join(os.scriptdir(), "test/test_random.xml")}, run_timeout = 1000, pass_outputs = "Succeed"})
        add_tests("test_sequence_selector", {runargs = {"--test_file", path.join(os.scriptdir(), "test/test_sequence_selector.xml")}, run_timeout = 1000, pass_outputs = "1456"})
        add_tests("test_repeat", {runargs = {"--test_file", path.join(os.scriptdir(), "test/test_repeat.xml")}, run_timeout = 1000, pass_outputs = "11111"})
        add_tests("test_sequence_repeat", {runargs = {"--test_file", path.join(os.scriptdir(), "test/test_sequence_repeat.xml")}, run_timeout = 1000, pass_outputs = "!11111!"})
        add_tests("test_repeat_sequence", {runargs = {"--test_file", path.join(os.scriptdir(), "test/test_repeat_sequence.xml")}, run_timeout = 1000, pass_outputs = "123123123123123"})
        add_tests("test_repeat_selector", {runargs = {"--test_file", path.join(os.scriptdir(), "test/test_repeat_selector.xml")}, run_timeout = 1000, pass_outputs = "11111"})
        add_tests("test_repeat_break_on_fail", {runargs = {"--test_file", path.join(os.scriptdir(), "test/test_repeat_break_on_fail.xml")}, run_timeout = 1000, pass_outputs = "1"})
        add_tests("test_sequence_double_repeat", {runargs = {"--test_file", path.join(os.scriptdir(), "test/test_sequence_double_repeat.xml")}, run_timeout = 1000, pass_outputs = "!11111!11111"})
        add_tests("test_sequence_repeat_double", {runargs = {"--test_file", path.join(os.scriptdir(), "test/test_sequence_repeat_double.xml")}, run_timeout = 1000, pass_outputs = "!1111111111111111111111111!"})
    end