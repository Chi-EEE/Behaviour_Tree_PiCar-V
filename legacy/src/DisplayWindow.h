#ifndef DISPLAYWINDOW_H
#define DISPLAYWINDOW_H

#pragma once

#include <SFML/Graphics.hpp>

#include <functional>
#include <vector>

#include "effolkronium/random.hpp"

#include "LidarScanner.h"

class DisplayWindow
{
public:
    DisplayWindow();
    ~DisplayWindow();
    void run();
    
private:
    void process_events();
    void update(sf::Time delta_time);
    void render();

    LidarScanner lidar_scanner;

    sf::RenderWindow window;
    sf::View view;
    bool exit_window = false;
    
    bool is_dragging = false;
    
    sf::Vector2f previous_mouse_position;
    
    float zoom = 1.0f;

    std::vector<std::vector<ScanPoint>> shapes;
};

#endif