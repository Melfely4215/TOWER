#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include <ctime>
#include "enemy.h"
#include "turret.h"
#include "wave.h"

void attackHandle(bool& drawAttack, sf::Vector2i& attackLocationInt, sf::CircleShape& attackCircle, std::vector<Enemy>& enemies) {
    //variables
        static std::array<sf::Vector2f, 2> attackArea;
        drawAttack = true;
        sf::Vector2f attackLocation = static_cast<sf::Vector2f>(attackLocationInt);
        sf::Vector2f enemyPos;
    //Logic
       
        attackCircle.setPosition(attackLocation);
    
        attackArea[0] = sf::Vector2f(attackCircle.getPosition().x - attackCircle.getRadius()*1.5, attackCircle.getPosition().y - attackCircle.getRadius()*1.5);
        attackArea[1] = sf::Vector2f(attackCircle.getPosition().x + attackCircle.getRadius()*1.5, attackCircle.getPosition().y + attackCircle.getRadius()*1.5);

        for (auto it = enemies.begin(); it != enemies.end();) {
            enemyPos = it->currentPos();
            if (enemyPos.x >= attackArea[0].x && enemyPos.y >= attackArea[0].y && enemyPos.x <= attackArea[1].x && enemyPos.y <= attackArea[1].y) {
                it->updateHp(25, 0);
                it++;
            }
            else {
                it++;
            }
        }
}

int main()
{
    std::srand(time(0));
    Wave waves;
    sf::RenderWindow window(sf::VideoMode({ 1920, 1080 }), "Tower Defense Game");
    window.setVerticalSyncEnabled(true); // Enable V-Sync
    //window.setFramerateLimit(60);
    sf::Vector2 windowSize = window.getSize();

    // Define waypoints for the path
    std::vector<sf::Vector2f> waypoints = {
        {0.f,  windowSize.y * 0.10f },
        {windowSize.x * 0.90f, windowSize.y * 0.10f},
        {windowSize.x * 0.90f, windowSize.y * 0.30f},
        {windowSize.x * 0.10f, windowSize.y * 0.30f},
        {windowSize.x * 0.10f, windowSize.y * 0.80f},
        {windowSize.x * 1.0f, windowSize.y * 0.80f},
        
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
    framerateText.setPosition({0.0f, windowSize.y * .96f});
    //Create an object to display enemy count and wave number
    sf::Text waveInfo(font);
    waveInfo.setCharacterSize(32);
    waveInfo.setFillColor(sf::Color::White);
    waveInfo.setPosition({ 0.0f, 20.0f });
    //Create attack shape
        sf::CircleShape attackCircle;
        attackCircle.setRadius(50.0f);
        attackCircle.setOutlineThickness(2.0f);
        attackCircle.setOutlineColor(sf::Color::Red);
        attackCircle.setFillColor(sf::Color::Transparent);
        attackCircle.setOrigin(attackCircle.getGeometricCenter());
        bool drawAttack = false;
        sf::Vector2i attackLocation;

    // Create a list of enemies
    std::vector<Enemy> enemies;
    //Create a list of Turrets
    std::vector<Turret> turrets;
    // Variables to control which half to update
    //Total Enemy Count
    int count = 0;
    //Spawn Delay
    float spawn = 0;
    bool buffer = false;
    

    // Main game loop
    sf::Clock clock; // to track time for consistent movement

    while (window.isOpen())
    {
        // Time since last frame
        sf::Time deltaTime = clock.restart();
        
        // Add an enemy to the list
        
        

        // Process events
        while (auto event = window.pollEvent())
        {
            if (event.has_value())
            {
                if (event->is<sf::Event::Closed>())
                    window.close();
                
                if (const auto& mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouseEvent->button == sf::Mouse::Button::Left) {
                        attackLocation = mouseEvent->position;
                        attackHandle(drawAttack, attackLocation, attackCircle, enemies);
                        
                    }
                }

                
                
            }

           
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1) && buffer == false) {
            sf::Vector2f mousePos;
            mousePos.x = sf::Mouse::getPosition(window).x;
            mousePos.y = sf::Mouse::getPosition(window).y;
            turrets.push_back(Turret(mousePos, 25, 5.0f, 3, sf::Color::Red, sf::Color::Blue, 0.0f, 1, 200.0f));
            buffer = true;
        }
        else if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1))) {
            buffer = false;
        }

        for (auto turret = turrets.begin(); turret != turrets.end();) {
            turret->shoot(deltaTime, enemies);
            turret++;
        }
        
        // Check if any enemy has reached the end of the path and remove them
        for (auto it = enemies.begin(); it != enemies.end();)
        {
            it->update(deltaTime);
            
            if (it->dead() ) {
                it = enemies.erase(it);
                waves.enemyDied();
                --count;
            }
            
            else if (it->hasReachedEnd())
            {
                it = enemies.erase(it); // Remove the enemy from the list
                waves.enemyDied();
                --count;
            }
            else
            {
                ++it; // Move to the next enemy
            }
        }

        

        waves.spawnEnemies(deltaTime, enemies, count, waypoints);
        waves.updateInfo(deltaTime);
        // Update UI text
        float fps = 1.f / deltaTime.asSeconds();
        framerateText.setString("FPS: " + std::to_string(static_cast<int>(fps)) );
        waveInfo.setString("Wave: " + std::to_string(waves.wave_Id()) + " Enemies Left: " + std::to_string(waves.enemy_Count()));

        // Clear the screen
        window.clear();

        // Draw the path
        window.draw(line);

        // Draw the framerate text
        window.draw(framerateText);
        window.draw(waveInfo);
        if (drawAttack == true) {
            window.draw(attackCircle);
            drawAttack = false;
        }

        // Draw the enemy
        for (const auto& enemy : enemies)
        {
            window.draw(enemy.getBody());
            window.draw(enemy.getHpBar());
        }
        //Draw Turrets
        for (const auto& turret : turrets) {
            window.draw(turret.getHull());
            window.draw(turret.getRange());
            if (turret.getShotTime() <= .05) {
                window.draw(turret.getShot());
                window.draw(turret.getShotAoe());
            }
            
        }

        // Display what was drawn
        window.display();
    }

    return 0;
}
