#ifndef TURRET_H
#define TURRET_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "enemy.h"


class Turret {
private:
    //Turret Stats
    int damage;
    float fireDelay;
    float lastShot = -1;
    int pointsCount;
    int ammo = -1;
    float reloadDelay = 0;
    int cost;
    float aoeSize;
    float range;

    //Rendering Data
    sf::CircleShape rangeDraw;
    sf::CircleShape hull;
    sf::Color hullColor;
    sf::VertexArray shot;
    sf::Color shotColor;
    sf::Vector2f location;
    sf::CircleShape shotAoe;

public:
    Turret(sf::Vector2f location, int damage, float shotsPerSec, int pointsCount, sf::Color hullColor, sf::Color shotColor, float aoeSize, int cost, float range);

    Turret(sf::Vector2f location, int damage, float shotsPerSec, int pointsCount, sf::Color hullColor, sf::Color shotColor, float aoeSize, int cost, float range, int ammo, float reloadDelay);

    void shoot(sf::Time deltaTime, std::vector<Enemy>& enemies);

    const sf::CircleShape& getHull() const;

    const sf::CircleShape& getRange() const;

    const sf::VertexArray& getShot() const;

    const sf::CircleShape& getShotAoe() const;

    const float& getShotTime() const;



};

#endif 