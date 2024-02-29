package("daemonpp-fork")
    set_base("daemonpp")

    add_patches("2023.05.01", path.join(os.scriptdir(), "patches", "2023.05.01", "add_inih.patch"), "3a2c3aa8cddf6d961e0823d5d4c429e9e03fa9c66b622235221070cea8d902a5")

    add_deps("inih")

    on_test(function (package)
        assert(package:check_cxxsnippets({test = [[
            #include <chrono>
            #include <daemonpp/daemon.hpp>

            using namespace daemonpp;

            class my_daemon : public daemon
            {
            public:
                void on_start(const INIReader reader) override {}
                void on_update() override {}
                void on_stop() override {}
                void on_reload(const INIReader reader) override {}
            };

            int test(int argc, const char* argv[]) {
                my_daemon dmn;
                dmn.set_name("my_daemon");
                dmn.set_update_duration(std::chrono::seconds(3));
                dmn.set_cwd("/");
                dmn.run(argc, argv);
                return 0;
            }
        ]]}, {configs = {languages = "cxx11"}}))
    end)
