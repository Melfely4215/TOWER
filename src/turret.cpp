#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include "turret.h"
#include "enemy.h"



    Turret::Turret(const std::vector<sf::Vector2f>& path, sf::Vector2f location, int damage, float shotsPerSec, int pointsCount, sf::Color hullColor, sf::Color shotColor, float aoeSize, int cost, float range)
        : path(path), location(location), damage(damage), fireDelay(1 / shotsPerSec), pointsCount(pointsCount), hullColor(hullColor), 
        shotColor(shotColor), cost(cost), aoeSize(aoeSize), range(range) {
        shot.setPrimitiveType(sf::PrimitiveType::LineStrip);
        shot.resize(2);
        hull.setPointCount(pointsCount);
        hull.setFillColor(hullColor);
        hull.setRadius(20.0f);
        hull.setOrigin(hull.getGeometricCenter());
        hull.setPosition(location);
        rangeDraw.setRadius(range);
        rangeDraw.setOrigin(rangeDraw.getGeometricCenter());
        rangeDraw.setOutlineColor(sf::Color::White);
        rangeDraw.setOutlineThickness(2.0f);
        rangeDraw.setPosition(location);
        rangeDraw.setFillColor(sf::Color::Transparent);
        shotAoe.setFillColor(sf::Color::Transparent);
        shotAoe.setRadius(aoeSize);
        shotAoe.setOutlineColor(shotColor);
        shotAoe.setOutlineThickness(2.0f);
        shotAoe.setOrigin(shotAoe.getGeometricCenter());
        shotAoe.setPosition(sf::Vector2f(-200, -200));
        rangeOptim = this->optimizeTargets();

        shot[0].position = location;
        shot[0].color = shotColor;
        shot[1].color = shotColor;
        shot[1].position = location;
    }

    Turret::Turret(const std::vector<sf::Vector2f>& path, sf::Vector2f location, int damage, float shotsPerSec, int pointsCount, sf::Color hullColor, sf::Color shotColor, float aoeSize, int cost, float range, int ammo, float reloadDelay)
        : path(path), location(location), damage(damage), fireDelay(1 / shotsPerSec), pointsCount(pointsCount), hullColor(hullColor), ammo(ammo), reloadDelay(reloadDelay),
            shotColor(shotColor), cost(cost), aoeSize(aoeSize), range(range) {
            shot.setPrimitiveType(sf::PrimitiveType::LineStrip);
            shot.resize(2);
            hull.setPointCount(pointsCount);
            hull.setFillColor(hullColor);
            hull.setRadius(20.0f);
            hull.setOrigin(hull.getGeometricCenter());
            hull.setPosition(location);
            rangeDraw.setRadius(range);
            rangeDraw.setOrigin(rangeDraw.getGeometricCenter());
            rangeDraw.setOutlineColor(sf::Color::White);
            rangeDraw.setOutlineThickness(2.0f);
            rangeDraw.setPosition(location);
            rangeDraw.setFillColor(sf::Color::Transparent);
            shotAoe.setFillColor(sf::Color::Transparent);
            shotAoe.setRadius(aoeSize);
            shotAoe.setOutlineColor(shotColor);
            shotAoe.setOutlineThickness(2.0f);
            shotAoe.setOrigin(shotAoe.getGeometricCenter());
            shotAoe.setPosition(sf::Vector2f(-200, -200));
            rangeOptim = this->optimizeTargets();

            shot[0].position = location;
            shot[0].color = shotColor;
            shot[1].color = shotColor;
            shot[1].position = location;
    }


    void Turret::shoot(sf::Time deltaTime, std::vector<Enemy>& enemies) {
        if (lastShot >= fireDelay) {
            int bestEnemy = -1;
            int counter = 0;
            float longDistanceTravl = 0;
            float currentDistTravl;
            for (auto it = enemies.begin(); it != enemies.end(); it++) {

                if (it->isDead() == 0) {
                    currentDistTravl = it->distanced_Traveled();
                    for (int i = 0; i < rangeOptim.size() - 1; i++ ) {
                        
                            if (rangeOptim[i] <= currentDistTravl && rangeOptim[i + 1] >= currentDistTravl) {
                                if (currentDistTravl > longDistanceTravl) {
                                    bestEnemy = counter;
                                    longDistanceTravl = currentDistTravl;
                                }
                            }
                        
                        
                    }
                    
                }
                counter++;
            }
            if (!(bestEnemy == -1)) {
                lastShot = 0;
                sf::Vector2f enemyLocation = enemies[bestEnemy].currentPos();
                shot[1].position = enemyLocation;
                shotAoe.setPosition(enemyLocation);
                if (aoeSize > 0) {

                    static std::array<sf::Vector2f, 2> attackArea;

                    attackArea[0] = sf::Vector2f(enemyLocation.x - aoeSize * 1.5, enemyLocation.y - aoeSize * 1.5);
                    attackArea[1] = sf::Vector2f(enemyLocation.x + aoeSize * 1.5, enemyLocation.y + aoeSize * 1.5);

                    for (auto it = enemies.begin(); it != enemies.end();) {
                        sf::Vector2f enemyPos = it->currentPos();
                        if (enemyPos.x >= attackArea[0].x && enemyPos.y >= attackArea[0].y && enemyPos.x <= attackArea[1].x && enemyPos.y <= attackArea[1].y) {
                            it->updateHp(damage, 0);
                            it++;
                        }
                        else {
                            it++;
                        }
                    }
                }
                else {
                    enemies[bestEnemy].updateHp(damage, 0);
                }

            }


        }
        lastShot += deltaTime.asSeconds();
    }

    void Turret::updateTurret(sf::Vector2f location, Turret turret) {
        hull.setPosition(location);
        hull.setFillColor(turret.hullColor);
        hull.setPointCount(turret.pointsCount);
        rangeDraw.setRadius(turret.range);
        rangeDraw.setOrigin(rangeDraw.getGeometricCenter());
        rangeDraw.setPosition(location);

    }

    std::vector<float> Turret::optimizeTargets() {
        std::vector<float> optimizeTargets; //Array of targets
        int currentTargetIndex = 1; //Current Target Index
        float distanceTotal = 0; //Total Distance Travelled
        float distance; //Linear distance to a target
        sf::Vector2f currentPos = path[0]; //Add up position.
        bool swap = false; //Swap between in and out points.
        sf::Vector2f targetPos;
        sf::Vector2f direction;

        float pathLengthDist; //Length of path chunk

        pathLengthDist = abs(path[currentTargetIndex].x - path[currentTargetIndex - 1].x) + abs(path[currentTargetIndex].y - path[currentTargetIndex - 1].y); 


        for (currentTargetIndex; currentTargetIndex < path.size(); currentTargetIndex++) {
            for (int i = 1; i < pathLengthDist; i++) {
                pathLengthDist = abs(path[currentTargetIndex].x - path[currentTargetIndex - 1].x) + abs(path[currentTargetIndex].y - path[currentTargetIndex - 1].y);
                targetPos = path[currentTargetIndex]; //Target Pos of the path
                direction = targetPos - currentPos; //Vector to path target
                distance = abs(direction.x) + abs(direction.y); //Get the magnitude

                if ((distance > 0)) {
                    direction = direction / distance; //remove mag from direction
                } 
                currentPos += direction; //Add the direction to current location
                distanceTotal += 1; //Add up the total


                direction = currentPos - location; //Turret to Current test location
                distance = abs(direction.x) + abs(direction.y); //Get magnitude

                if (!(distance >= range) && swap == false) { 
                    optimizeTargets.push_back(distanceTotal); //Add the point entering us into range
                    swap = true;

                }
                else if ((distance >= range && swap == true)) {
                    optimizeTargets.push_back(distanceTotal - 1); //Add the point leaving the part of range
                    swap = false;

                }

            }
        }
        
        return optimizeTargets;
    }
    

    

    const sf::CircleShape& Turret::getHull() const { return hull; }

    const sf::CircleShape& Turret::getRange() const { return rangeDraw; }

    const sf::VertexArray& Turret::getShot() const { return shot; }

    const sf::CircleShape& Turret::getShotAoe() const { return shotAoe; }

    const float& Turret::getShotTime() const { return lastShot; }

    const int& Turret::returnDamage() const { return damage; }

    const float& Turret::returnSPS() const { return fireDelay; }

    const int& Turret::returnCost() const { return cost; }

    const float& Turret::returnAoe() const { return aoeSize; }

    const float& Turret::returnRange() const { return range; }

    const int& Turret::returnAmmo() const { return ammo; }

    const float& Turret::returnReload() const { return reloadDelay; }