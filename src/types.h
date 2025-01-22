#ifndef TYPES_H
#define TYPES_H

#include "turret.h"
#include "enemy.h"


//Turrets 
    class Basic_Turret : public Turret {
    public:
        Basic_Turret(sf::Vector2f location);


    };

    class Empty_Turret : public Turret {
    public:
        Empty_Turret(sf::Vector2f location);


    };

    class Bomb_Turret : public Turret {
    public:
        Bomb_Turret(sf::Vector2f location);
    };

//Enemies
    class Basic_Enemy : public Enemy {
    public: 
        Basic_Enemy(const std::vector<sf::Vector2f>& waypoints);
    };

    class Fast_Enemy : public Enemy {
    public:
        Fast_Enemy(const std::vector<sf::Vector2f>& waypoints);
    };

#endif