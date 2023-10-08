#include <iostream>

#include <vector>
#include <unordered_map>

#include <fmt/format.h>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "RPLidar.h"

struct Point
{
    std::shared_ptr<sf::VertexArray> lines;
};

struct Vector2fHash
{
    std::size_t operator()(const sf::Vector2f &v) const
    {
        std::size_t seed = 0;
        hash_combine(seed, std::hash<float>{}(v.x));
        hash_combine(seed, std::hash<float>{}(v.y));
        return seed;
    }

    template <typename T>
    void hash_combine(std::size_t &seed, const T &val) const
    {
        seed ^= std::hash<T>{}(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
};

bool operator==(const sf::Vector2f &lhs, const sf::Vector2f &rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

int main()
{
    spdlog::set_level(spdlog::level::off);
    auto lidar = RPLidar("COM3");
    lidar.stop();
    lidar.stop_motor();

    std::function<std::vector<Measure>()> scanGenerator = lidar.iter_scans();

    sf::RenderWindow window(sf::VideoMode(1000, 1000), "My window");

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        sf::Vector2f center(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
        sf::CircleShape point(5.f); // Change the radius as needed
        point.setFillColor(sf::Color::Green);
        point.setPosition(center);
        window.draw(point);
        std::vector<Measure> scan = scanGenerator();
        std::cout << "Got " << scan.size() << " Measures!\n";

        std::unordered_map<sf::Vector2f, Point, Vector2fHash> point_map;
        std::vector<std::shared_ptr<sf::VertexArray>> line_vector;

        for (int i = 0; i < scan.size(); i++)
        {
            const Measure &measure = scan[i];
            float angle = measure.angle;
            float distance = measure.distance * 0.3;
            float angleInRadians = angle * (3.14159265f / 180.0f);
            float x = distance * std::cos(angleInRadians);
            float y = distance * std::sin(angleInRadians);
            point_map.insert({{x, y}, Point{}});
        }
        for (int i = 0; i < scan.size(); i++)
        {
            const Measure &previous_measure = scan[((i - 1 % scan.size()) + scan.size()) % scan.size()];
            const Measure &measure = scan[i];
            const Measure &next_measure = scan[(i + 1) % scan.size()];

            bool previous_newScan = previous_measure.newScan;
            int previous_quality = previous_measure.quality;
            float previous_angle = previous_measure.angle;
            float previous_distance = previous_measure.distance * 0.3;

            bool newScan = measure.newScan;
            int quality = measure.quality;
            float angle = measure.angle;
            float distance = measure.distance * 0.3;

            bool next_newScan = next_measure.newScan;
            int next_quality = next_measure.quality;
            float next_angle = next_measure.angle;
            float next_distance = next_measure.distance * 0.3;

            // Convert degrees to radians
            float previous_angleInRadians = previous_angle * (3.14159265f / 180.0f);
            float angleInRadians = angle * (3.14159265f / 180.0f);
            float next_angleInRadians = next_angle * (3.14159265f / 180.0f);

            float previous_x = distance * std::cos(previous_angleInRadians);
            float previous_y = distance * std::sin(previous_angleInRadians);

            float x = distance * std::cos(angleInRadians);
            float y = distance * std::sin(angleInRadians);

            float next_x = next_distance * std::cos(next_angleInRadians);
            float next_y = next_distance * std::sin(next_angleInRadians);

            sf::Vector2f previous_point = {center.x + previous_x, center.y + previous_y};
            sf::Vector2f point = {center.x + x, center.y + y};
            sf::Vector2f next_point = {center.x + next_x, center.y + next_y};

            std::cout << fmt::format("Distance: {}\n", std::abs(distance - next_distance));
            if (std::abs(distance - next_distance) < 20)
            {
                if (point_map[{next_point.x, next_point.y}].lines != nullptr)
                {
                    point_map[{next_point.x, next_point.y}].lines->append(point);
                }
                else
                {
                    std::shared_ptr<sf::VertexArray> lines = point_map[{point.x, point.y}].lines;
                    if (lines == nullptr)
                    {
                        lines = std::make_shared<sf::VertexArray>(sf::VertexArray{sf::LinesStrip});
                        point_map[{point.x, point.y}].lines = lines;

                        sf::Vertex vertex{point};
                        vertex.color = sf::Color::Green;

                        lines->append(vertex);
                        line_vector.push_back(lines);
                    }
                    if (point_map[{next_point.x, next_point.y}].lines == nullptr)
                    {
                        point_map[{next_point.x, next_point.y}].lines = lines;
                        
                        sf::Vertex vertex{next_point};
                        vertex.color = sf::Color::Green;

                        lines->append(vertex);
                    }
                }
            }
        }
        for (auto &lines : line_vector)
        {
            window.draw(*lines);
        }

        // end the current frame
        window.display();
    }

    lidar.stop();
    lidar.stop_motor();
    lidar.disconnect();
}