#include <iostream>
#include <chrono>
#include <daemonpp/daemon.hpp>

using namespace daemonpp;

class my_daemon : public daemon
{
public:
    void on_start(const INIReader reader) override {
        std::cout << "on_start" << std::endl;
    }
    void on_update() override {}
    void on_stop() override {}
    void on_reload(const INIReader reader) override {}
};

int main(int argc, const char* argv[]) {
    my_daemon dmn;
    dmn.set_name("my_daemon");
    dmn.set_update_duration(std::chrono::seconds(3));
    dmn.set_cwd("/");
    dmn.run(argc, argv);
    return 0;
}