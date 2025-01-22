#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include "turret.h"
#include "enemy.h"

    Turret::Turret(sf::Vector2f location, int damage, float shotsPerSec, int pointsCount, sf::Color hullColor, sf::Color shotColor, float aoeSize, int cost, float range)
        : location(location), damage(damage), fireDelay(1 / shotsPerSec), pointsCount(pointsCount), hullColor(hullColor), shotColor(shotColor), cost(cost), aoeSize(aoeSize), range(range) {
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

        shot[0].position = location;
        shot[0].color = shotColor;
        shot[1].color = shotColor;
        shot[1].position = location;
    }

    Turret::Turret(sf::Vector2f location, int damage, float shotsPerSec, int pointsCount, sf::Color hullColor, sf::Color shotColor, float aoeSize, int cost, float range, int ammo, float reloadDelay)
        : damage(damage), fireDelay(1 / shotsPerSec), pointsCount(pointsCount), ammo(ammo), reloadDelay(reloadDelay), hullColor(hullColor), shotColor(shotColor), cost(cost), aoeSize(aoeSize), range(range) {
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

        shot[0].color = shotColor;
        shot[1].color = shotColor;
        shot[0].position = location;
        shot[1].position = location;

    }

    void Turret::shoot(sf::Time deltaTime, std::vector<Enemy>& enemies) {
        if (lastShot >= fireDelay) {
            int bestEnemy = -1;
            float currentBestDist = range + 1;
            int counter = 0;
            float longDistanceTravl = 0;
            for (auto it = enemies.begin(); it != enemies.end();) {


                sf::Vector2f currentPosition = hull.getPosition();
                sf::Vector2f targetPosition = it->currentPos();
                sf::Vector2f direction = targetPosition - currentPosition;
                float distance = sqrt(direction.x * direction.x + direction.y * direction.y);


                if (!(distance >= range) && it->distanced_Traveled() > longDistanceTravl) {
                    currentBestDist = distance;
                    longDistanceTravl = it->distanced_Traveled();
                    bestEnemy = counter;
                }
                counter++;
                it++;

            }
            if (currentBestDist <= range && !(bestEnemy == -1)) {
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

    const sf::CircleShape& Turret::getHull() const {
        return hull;
    }

    const sf::CircleShape& Turret::getRange() const {
        return rangeDraw;
    }

    const sf::VertexArray& Turret::getShot() const {
        return shot;
    }

    const sf::CircleShape& Turret::getShotAoe() const {
        return shotAoe;
    }

    const float& Turret::getShotTime() const {
        return lastShot;
    }
