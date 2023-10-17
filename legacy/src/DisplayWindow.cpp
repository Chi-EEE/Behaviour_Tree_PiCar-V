#include "DisplayWindow.h"

DisplayWindow::DisplayWindow() : window(sf::VideoMode(1000, 1000), "My window"), lidar_scanner("COM3")
{
}

DisplayWindow::~DisplayWindow()
{
}

void DisplayWindow::run()
{
    sf::Clock clock;
    sf::Time time_since_last_update = sf::Time::Zero;
    sf::Time ticks_per_second = sf::seconds(1.0f / 60.0f);
    while (window.isOpen())
    {
        this->process_events();
        time_since_last_update += clock.restart();
        while (time_since_last_update > ticks_per_second)
        {
            time_since_last_update -= ticks_per_second;
            this->process_events();
            update(ticks_per_second);
        }
        render();
    }
}

void DisplayWindow::process_events()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            this->exit_window = true;

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {
            is_dragging = true;
            previous_mouse_position = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        }
        else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
        {
            is_dragging = false;
        }
        else if (event.type == sf::Event::MouseMoved && is_dragging)
        {
            sf::Vector2f current_mouse_position = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            sf::Vector2f delta = previous_mouse_position - current_mouse_position;
            view.move(delta);
            previous_mouse_position = current_mouse_position;
        }
        else if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Add && event.key.control)
            {
                zoom = std::max(zoom - 0.1f, 0.1f);
                view.zoom(zoom);
            }
            else if (event.key.code == sf::Keyboard::Subtract && event.key.control)
            {
                zoom = zoom + 0.1f;
                view.zoom(zoom);
            }
        }
    }
}

void DisplayWindow::update(sf::Time delta_time)
{
    if (this->exit_window)
    {
        this->window.close();
    }
    this->shapes = this->lidar_scanner.scan();
}

void DisplayWindow::render()
{
    this->window.clear(sf::Color::White);
    for (auto &shape : this->shapes)
    {
        sf::Color color = sf::Color::Color(effolkronium::random_static::get(0U, UINT_MAX));
        sf::VertexArray vertex_array(sf::Lines, shape.size());
        for (int i = 0; i < shape.size(); i++)
        {
            auto &point = shape[i];
            vertex_array[i] = sf::Vertex(sf::Vector2f(point.x, point.y), color);
        }
        this->window.draw(vertex_array);
    }
    this->window.display();
}
