#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include <ctime>
#include "enemy.h"
#include "turret.h"
#include "wave.h"

//struct turret_Type {
//    int damage;
//    float shotsPerSec;
//    int pointsCount;
//    sf::Color hullColor;
//    sf::Color shotColor;
//    float aoeSize;
//    int cost;
//    float range;
//    int ammo = -1;
//    float reloadDelay = 0;
//};

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

void turretPrev(Turret& prevTurret, sf::RenderWindow& window, Turret::turret_Type prev) {
    sf::Vector2f mousePos;
    mousePos.x = sf::Mouse::getPosition(window).x;
    mousePos.y = sf::Mouse::getPosition(window).y;
    prevTurret.prevUp(mousePos, prev);
}

void turretPlace(Wave& waves, sf::RenderWindow& window, std::vector<Turret>& turrets, Turret::turret_Type turret) {
    if (waves.returnMoney() >= turret.cost) {
        waves.spendMoney(turret.cost);
        sf::Vector2f mousePos;
        mousePos.x = sf::Mouse::getPosition(window).x;
        mousePos.y = sf::Mouse::getPosition(window).y;
        turrets.push_back(Turret(mousePos, turret));

    }
    else {


    }
}

void keyboardInputs(std::vector<Turret>& turrets, sf::RenderWindow& window, Wave& waves, const std::optional<sf::Event>& event, Turret& prevTurret){
    const Turret::turret_Type emptyTurret = {
        0,              // damage
        0.0f,            // shotsPerSec
        0,               // pointsCount
        sf::Color::Transparent, // hullColor
        sf::Color::Transparent,   // shotColor
        0.0f,            // aoeSize
        0,             // cost
        0.0f            // range
    };
    
    const Turret::turret_Type basicTurret = {
        25,              // damage
        4.0f,            // shotsPerSec
        3,               // pointsCount
        sf::Color::Red, // hullColor
        sf::Color::Blue,   // shotColor
        0.0f,            // aoeSize
        10,             // cost
        200.0f            // range
    };

    const Turret::turret_Type bombTurret = {
        50,              // damage
        0.5f,            // shotsPerSec
        5,               // pointsCount
        sf::Color::Green, // hullColor
        sf::Color::Red,   // shotColor
        100.0f,            // aoeSize
        50,             // cost
        300.0f            // range
    };
    static const sf::Vector2f emptyLoc = { 0,0 };
    
    if (event->is<sf::Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Num1) {
        turretPrev(prevTurret, window, basicTurret);

    } else if (event->is<sf::Event::KeyReleased>() && event->getIf<sf::Event::KeyReleased>()->code == sf::Keyboard::Key::Num1) {
        prevTurret.prevUp(emptyLoc, emptyTurret);
        turretPlace(waves, window, turrets, basicTurret);

    }

    if (event->is<sf::Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Num2) {
        turretPrev(prevTurret, window, bombTurret);

    }
    else if (event->is<sf::Event::KeyReleased>() && event->getIf<sf::Event::KeyReleased>()->code == sf::Keyboard::Key::Num2) {
        prevTurret.prevUp(emptyLoc, emptyTurret);
        turretPlace(waves, window, turrets, bombTurret);

    }
}

int main()
{
    //Initialzation
        std::srand(time(0));
        Wave waves;
        sf::RenderWindow window(sf::VideoMode({ 1920, 1080 }), "Tower Defense Game");
        window.setVerticalSyncEnabled(false); // Enable V-Sync
        //window.setFramerateLimit(60);
        sf::Vector2 windowSize = window.getSize();
        std::vector<Enemy> enemies; //List of enemies
        std::vector<Turret> turrets; //List of turrets
        Turret prevTurret = {sf::Vector2f(0,0), 0, 0.0f, 0, sf::Color::Transparent, sf::Color::Transparent, 0.0f, 0, 0};
        int count = 0; //Total Enemy Count

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


    
    

    // Main game loop
    sf::Clock clock; // to track time for consistent movement

    while (window.isOpen())
    {
        // Time since last frame
        sf::Time deltaTime = clock.restart();
        
        

        // Process events
        while (const std::optional event = window.pollEvent())
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

                keyboardInputs(turrets, window, waves, event, prevTurret);
                
            }

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
                waves.enemyDied(it->enemyValue());
                it = enemies.erase(it);
                --count;
            }
            
            else if (it->hasReachedEnd())
            {
                waves.enemyDied(it->enemyValue());
                it = enemies.erase(it); // Remove the enemy from the list
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
        waveInfo.setString("Wave: " + std::to_string(waves.wave_Id()) + " Enemies Left: " + std::to_string(waves.enemy_Count()) + " Money: $" + std::to_string(waves.returnMoney()));

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
        //Draw Prev Turret
        window.draw(prevTurret.getHull());
        window.draw(prevTurret.getRange());
        

        // Display what was drawn
        window.display();
    }

    return 0;
}
