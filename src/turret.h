#ifndef TURRET_H
#define TURRET_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "enemy.h"


class Turret {
private:
    //Turret Stats
    int damage; //Damage of each shot
    float fireDelay; //Delay between each shot
    float lastShot = -1; //How long since last shot
    int pointsCount; //Number of sides to shape min of 3
    int ammo = -1; //total ammo
    float reloadDelay = 0; //Reload time
    int cost; //Cost of turret
    float aoeSize; //Size of aoe attack
    float range; //Attack range
    std::vector<float> rangeOptim; //Dynamiclly allocated array of valid attack locations;
    const std::vector<sf::Vector2f> path; //The path enemies will follow for range optimization

    //Rendering Data
    sf::CircleShape rangeDraw;
    sf::CircleShape hull;
    sf::Color hullColor;
    sf::VertexArray shot;
    sf::Color shotColor;
    sf::Vector2f location;
    sf::CircleShape shotAoe;

public:

    Turret(const std::vector<sf::Vector2f>& path, sf::Vector2f location, int damage, float shotsPerSec, int pointsCount, sf::Color hullColor, sf::Color shotColor, float aoeSize, int cost, float range);
    Turret(const std::vector<sf::Vector2f>& path, sf::Vector2f location, int damage, float shotsPerSec, int pointsCount, sf::Color hullColor, sf::Color shotColor, float aoeSize, int cost, float range, int ammo, float reloadDelay);

    void shoot(sf::Time deltaTime, std::vector<Enemy>& enemies);

    void updateTurret(sf::Vector2f location, Turret turret);

    std::vector<float> optimizeTargets();

    const sf::CircleShape& getHull() const;

    const sf::CircleShape& getRange() const;

    const sf::VertexArray& getShot() const;

    const sf::CircleShape& getShotAoe() const;

    const float& getShotTime() const;

    const int& returnDamage() const;

    const float& returnSPS() const;

    const int& returnCost() const;

    const float& returnAoe() const;

    const float& returnRange() const;

    const int& returnAmmo() const;

    const float& returnReload() const;
    


};

#endif 