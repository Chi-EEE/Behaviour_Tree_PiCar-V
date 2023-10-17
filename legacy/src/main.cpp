#include <iostream>

#include <vector>
#include <unordered_map>

#include <fmt/format.h>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "DisplayWindow.h"

int main()
{
    spdlog::set_level(spdlog::level::off);
    DisplayWindow displayWindow;
    try {
        displayWindow.run();
    } catch(std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}