#include "turret.h"
#include "enemy.h"
#include "types.h"


//Turrets
    Basic_Turret::Basic_Turret(const std::vector<sf::Vector2f>& path, sf::Vector2f location)
        : Turret(path, location,
            25,              // damage
            4.0f,            // shotsPerSec
            3,               // pointsCount
            sf::Color::Red, // hullColor
            sf::Color::Blue,   // shotColor
            0.0f,            // aoeSize
            25,             // cost
            200.0f            // range
        ) {}

    Empty_Turret::Empty_Turret(const std::vector<sf::Vector2f>& path, sf::Vector2f location)
        : Turret(path, location,
            0,              // damage
            0.0f,            // shotsPerSec
            0,               // pointsCount
            sf::Color::Transparent, // hullColor
            sf::Color::Transparent,   // shotColor
            0.0f,            // aoeSize
            0,             // cost
            0.0f            // range
        ) {}

    Bomb_Turret::Bomb_Turret(const std::vector<sf::Vector2f>& path, sf::Vector2f location)
        : Turret(path, location,
            50,              // damage
            0.5f,            // shotsPerSec
            5,               // pointsCount
            sf::Color::Green, // hullColor
            sf::Color::Red,   // shotColor
            100.0f,            // aoeSize
            100,             // cost
            300.0f            // range
        ) {}

//Enemies
    Basic_Enemy::Basic_Enemy(const std::vector<sf::Vector2f>& waypoints)
        : Enemy(waypoints, 
            100, //Speed 
            100, //Hp
            25, //Size
            1, //Value
            sf::Color::White) //Color

    {}

    Fast_Enemy::Fast_Enemy(const std::vector<sf::Vector2f>& waypoints)
        : Enemy(waypoints,
            300, //Speed 
            25, //Hp
            15, //Size
            1, //Value
            sf::Color::Blue) //Color
    {}
