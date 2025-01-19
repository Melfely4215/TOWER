#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <iostream>

class Enemy
{
public:
    Enemy(const std::vector<sf::Vector2f>& path, float speed)
        : path(path), speed(speed), currentTargetIndex(1)
    {
        shape.setRadius(20.0f);
        shape.setFillColor(sf::Color::Red);
        shape.setOrigin(shape.getGeometricCenter());
        shape.setPosition(path[0]);
        
    }

    void update(sf::Time deltaTime)
    {
        if (currentTargetIndex < path.size())
        {
            sf::Vector2f currentPosition = shape.getPosition();
            sf::Vector2f targetPosition = path[currentTargetIndex];
            sf::Vector2f direction = targetPosition - currentPosition;
            float distance = abs(direction.x) + abs(direction.y);

            if (distance > speed * deltaTime.asSeconds())
            {
                direction /= distance;
                shape.move(direction * speed * deltaTime.asSeconds());
            }
            else
            {
                shape.setPosition(targetPosition); // Snap to the target position if close enough
                ++currentTargetIndex; // Move to the next target
            }
        }
    }

    bool hasReachedEnd() const
    {
        return currentTargetIndex >= path.size();
    }

    const sf::CircleShape& getShape() const
    {
        return shape;
    }

private:
    sf::CircleShape shape;
    std::vector<sf::Vector2f> path;
    float speed;
    std::size_t currentTargetIndex;
};

int main()
{

    sf::RenderWindow window(sf::VideoMode({ 1920, 1080 }), "Tower Defense Game");
    window.setVerticalSyncEnabled(false); // Enable V-Sync
    sf::Vector2 windowSize = window.getSize();

    // Define waypoints for the path
    std::vector<sf::Vector2f> waypoints = {
        {0.f,  windowSize.y * 0.10f },
        {windowSize.x * 0.80f, windowSize.y * 0.10f},
        {windowSize.x * 0.80f, windowSize.y * 0.30f},
        {windowSize.x * 0.10f, windowSize.y * 0.30f},
        {windowSize.x * 0.10f, windowSize.y * 0.60f},
        {windowSize.x * 0.80f, windowSize.y * 0.60f},
        {windowSize.x * 0.80f, windowSize.y * 1.0f}
    };

    // Set initial position of the enemy at the start of the path
    
    // Create a vertex array for the path
    sf::VertexArray line(sf::PrimitiveType::LineStrip, waypoints.size());
    for (std::size_t i = 0; i < waypoints.size(); ++i)
    {
        line[i].position = waypoints[i];
        line[i].color = sf::Color::Red;
    }

    // Load a font
    sf::Font font;
    if (!font.openFromFile("Roboto-Regular.ttf"))
    {
        std::cerr << "Could not load font" << std::endl;
    }

    
    // Create a text object to display the framerate
    sf::Text framerateText(font);
    framerateText.setCharacterSize(24);
    framerateText.setFillColor(sf::Color::White);
    framerateText.setPosition({0.0, 20.0f});

    // Create a list of enemies
    std::vector<Enemy> enemies;
    // Variables to control which half to update
    //Total Enemy Count
    int count = 0;
    //Spawn Delay
    float spawn = 0;

    

    // Main game loop
    sf::Clock clock; // to track time for consistent movement

    while (window.isOpen())
    {
        // Time since last frame
        sf::Time deltaTime = clock.restart();
        
        // Add an enemy to the list
        if (!(count >= 100) && spawn <= 0) {
            enemies.emplace_back(waypoints, 100); // speed = 100 pixels per second
            count++;
            spawn = 1;
        }
        else {
            spawn -= static_cast<float>(deltaTime.asMilliseconds()); 
        }
        

        // Process events
        while (auto event = window.pollEvent())
        {
            if (event.has_value())
            {
                if (event->is<sf::Event::Closed>())
                    window.close();
            }
        }
        

        // Check if any enemy has reached the end of the path and remove them
        for (auto it = enemies.begin(); it != enemies.end();)
        {
            it->update(deltaTime);
            if (it->hasReachedEnd())
            {
                it = enemies.erase(it); // Remove the enemy from the list
                --count;
            }
            else
            {
                ++it; // Move to the next enemy
            }
        }

        // Update framerate text
        float fps = 1.f / deltaTime.asSeconds();
        framerateText.setString("FPS: " + std::to_string(static_cast<int>(fps) ) + " Enemies: " + std::to_string(count));

        // Clear the screen
        window.clear();

        // Draw the path
        window.draw(line);

        // Draw the framerate text
        window.draw(framerateText);

        // Draw the enemy
        for (const auto& enemy : enemies)
        {
            window.draw(enemy.getShape());
        }
        // Display what was drawn
        window.display();
    }

    return 0;
}
