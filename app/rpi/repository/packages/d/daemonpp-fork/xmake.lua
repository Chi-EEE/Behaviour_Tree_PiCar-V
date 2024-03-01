package("daemonpp-fork")
    set_base("daemonpp")

    add_patches("2023.05.01", path.join(os.scriptdir(), "patches", "2023.05.01", "add_inih.patch"), "8af0d2a28484a8174a6c41a2c58705bc7032268b60f19d162c8fe8eb12c39af8")

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
