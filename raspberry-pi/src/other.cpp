// #include <iostream>

// #include <vector>
// #include <unordered_map>

// #include <fmt/format.h>
// #include <SFML/System.hpp>
// #include <SFML/Graphics.hpp>

// #include "RPLidar.h"

// struct Point
// {
//     std::shared_ptr<sf::VertexArray> lines;
// };

// struct Vector2fHash
// {
//     std::size_t operator()(const sf::Vector2f &v) const
//     {
//         std::size_t seed = 0;
//         hash_combine(seed, std::hash<float>{}(v.x));
//         hash_combine(seed, std::hash<float>{}(v.y));
//         return seed;
//     }

//     template <typename T>
//     void hash_combine(std::size_t &seed, const T &val) const
//     {
//         seed ^= std::hash<T>{}(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
//     }
// };

// bool operator==(const sf::Vector2f &lhs, const sf::Vector2f &rhs)
// {
//     return lhs.x == rhs.x && lhs.y == rhs.y;
// }

// int main()
// {
//     spdlog::set_level(spdlog::level::off);
//     auto lidar = RPLidar("COM3");
//     lidar.stop();
//     lidar.stop_motor();

//     std::function<std::vector<Measure>()> scanGenerator = lidar.iter_scans();

//     sf::RenderWindow window(sf::VideoMode(1000, 1000), "My window");
//     sf::View view;
//     bool is_dragging = false;
//     sf::Vector2f previous_mouse_position;
//     float zoom = 1.0f;

//     while (window.isOpen())
//     {
//         sf::Event event;
//         while (window.pollEvent(event))
//         {
//             if (event.type == sf::Event::Closed)
//                 window.close();

//             if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
//             {
//                 is_dragging = true;
//                 previous_mouse_position = window.mapPixelToCoords(sf::Mouse::getPosition(window));
//             }
//             else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
//             {
//                 is_dragging = false;
//             }
//             else if (event.type == sf::Event::MouseMoved && is_dragging)
//             {
//                 sf::Vector2f current_mouse_position = window.mapPixelToCoords(sf::Mouse::getPosition(window));
//                 sf::Vector2f delta = previous_mouse_position - current_mouse_position;
//                 view.move(delta);
//                 previous_mouse_position = current_mouse_position;
//             }
//             else if (event.type == sf::Event::KeyPressed)
//             {
//                 if (event.key.code == sf::Keyboard::Add && event.key.control)
//                 {
//                     zoom = std::max(zoom - 0.1f, 0.1f);
//                     view.zoom(zoom);
//                 }
//                 else if (event.key.code == sf::Keyboard::Subtract && event.key.control)
//                 {
//                     zoom = zoom + 0.1f;
//                     view.zoom(zoom);
//                 }
//             }
//         }

//         // clear the window with black color
//         window.clear(sf::Color::Black);
//         window.setView(view);

//         sf::Vector2f center(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
//         sf::CircleShape point(5.f); // Change the radius as needed
//         point.setFillColor(sf::Color::Green);
//         point.setPosition(center);
//         window.draw(point);
//         std::vector<Measure> scan = scanGenerator();
//         std::cout << "Got " << scan.size() << " Measures!\n";

//         std::unordered_map<sf::Vector2f, Point, Vector2fHash> point_map;
//         std::vector<std::shared_ptr<sf::VertexArray>> line_vector;

//         for (int i = 1; i < scan.size(); i++)
//         {
//             const Measure &measure = scan[i];
//             float angle = measure.angle;
//             float distance = measure.distance * 0.3;
//             float angleInRadians = angle * (3.14159265f / 180.0f);
//             float x = distance * std::cos(angleInRadians);
//             float y = distance * std::sin(angleInRadians);
//             point_map.insert({{x, y}, Point{}});
//         }
//         for (int i = 1; i < scan.size(); i++)
//         {
//             const Measure &previous_measure = scan[((i - 1 % scan.size()) + scan.size()) % scan.size()];
//             const Measure &measure = scan[i];
//             const Measure &next_measure = scan[(i + 1) % scan.size()];

//             bool previous_newScan = previous_measure.newScan;
//             int previous_quality = previous_measure.quality;
//             float previous_angle = previous_measure.angle;
//             float previous_distance = previous_measure.distance * 0.3;

//             bool newScan = measure.newScan;
//             int quality = measure.quality;
//             float angle = measure.angle;
//             float distance = measure.distance * 0.3;

//             bool next_newScan = next_measure.newScan;
//             int next_quality = next_measure.quality;
//             float next_angle = next_measure.angle;
//             float next_distance = next_measure.distance * 0.3;

//             // Convert degrees to radians
//             float previous_angleInRadians = previous_angle * (3.14159265f / 180.0f);
//             float angleInRadians = angle * (3.14159265f / 180.0f);
//             float next_angleInRadians = next_angle * (3.14159265f / 180.0f);

//             float previous_x = distance * std::cos(previous_angleInRadians);
//             float previous_y = distance * std::sin(previous_angleInRadians);

//             float x = distance * std::cos(angleInRadians);
//             float y = distance * std::sin(angleInRadians);

//             float next_x = next_distance * std::cos(next_angleInRadians);
//             float next_y = next_distance * std::sin(next_angleInRadians);

//             sf::Vector2f previous_point = {center.x + previous_x, center.y + previous_y};
//             sf::Vector2f point = {center.x + x, center.y + y};
//             sf::Vector2f next_point = {center.x + next_x, center.y + next_y};

//             float slope = (next_point.x - previous_point.x) / (next_point.y - previous_point.y);

//             int slope_y = (slope * point.x) + 0;
//             if (point.y + 60 <= slope_y && point.y - 60 >= slope_y)
//             {
//                 std::shared_ptr<sf::VertexArray> lines = point_map[{previous_point.x, previous_point.y}].lines;
//                 if (lines == nullptr)
//                 {
//                     lines = std::make_shared<sf::VertexArray>(sf::VertexArray{sf::LinesStrip});
//                     point_map[{previous_point.x, previous_point.y}].lines = lines;
//                     line_vector.push_back(lines);

//                     lines->append(sf::Vertex{previous_point, sf::Color::Green});
//                 }
//                 point_map[{point.x, point.y}].lines = lines;
//                 point_map[{next_point.x, next_point.y}].lines = lines;

//                 lines->append(sf::Vertex{point, sf::Color::Green});
//                 lines->append(sf::Vertex{next_point, sf::Color::Green});

//                 continue;
//             }

//             std::cout << fmt::format("Distance: {}\n", std::abs(distance - next_distance));
//             if (std::abs(distance - next_distance) < 20)
//             {
//                 if (point_map[{next_point.x, next_point.y}].lines != nullptr)
//                 {
//                     point_map[{next_point.x, next_point.y}].lines->append(point);
//                 }
//                 else
//                 {
//                     std::shared_ptr<sf::VertexArray> lines = point_map[{point.x, point.y}].lines;
//                     if (lines == nullptr)
//                     {
//                         lines = std::make_shared<sf::VertexArray>(sf::VertexArray{sf::LinesStrip});
//                         point_map[{point.x, point.y}].lines = lines;

//                         sf::Vertex vertex{point};
//                         vertex.color = sf::Color::Green;

//                         lines->append(vertex);
//                         line_vector.push_back(lines);
//                     }
//                     if (point_map[{next_point.x, next_point.y}].lines == nullptr)
//                     {
//                         point_map[{next_point.x, next_point.y}].lines = lines;

//                         sf::Vertex vertex{next_point};
//                         vertex.color = sf::Color::Green;

//                         lines->append(vertex);
//                     }
//                 }
//             }
//         }
//         for (auto &measure : scan)
//         {
//             bool newScan = measure.newScan;
//             int quality = measure.quality;
//             float angle = measure.angle;
//             float distance = measure.distance * 0.3;

//             // Convert degrees to radians
//             float angleInRadians = angle * (3.14159265f / 180.0f);

//             float x = distance * std::cos(angleInRadians);
//             float y = distance * std::sin(angleInRadians);

//             sf::Vector2f point = {center.x + x, center.y + y};

//             sf::Vertex vertices[2] = {
//                 sf::Vertex{center, sf::Color::Blue},
//                 sf::Vertex{point, sf::Color::Blue},
//             };

//             window.draw(vertices, 2, sf::Lines);
//         }
//         for (auto &lines : line_vector)
//         {
//             window.draw(*lines);
//         }

//         // end the current frame
//         window.display();
//     }

//     lidar.stop();
//     lidar.stop_motor();
//     lidar.disconnect();
// }