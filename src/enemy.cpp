#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include "enemy.h"

    //Class Data
    Enemy::Enemy(const std::vector<sf::Vector2f>& path, float speed, float hpTotal, float size, int value, sf::Color color)
        : path(path), speed(speed), currentTargetIndex(1), hp(hpTotal), size(size), value(value), color(color)
    {
        shape.setRadius(size);
        shape.setFillColor(color);
        shape.setOrigin(shape.getGeometricCenter());
        shape.setPosition(path[0]);
        currentHp = hp;
        hpShape.setOutlineColor(sf::Color::Black);
        hpShape.setOutlineThickness(2.0f);
        hpShape.setFillColor(sf::Color::Green);
        hpShape.setSize({ barSize, 10.0f });
        hpShape.setOrigin(hpShape.getGeometricCenter());
        //shape.setPointCount(5);


    }

    Enemy::Enemy(const std::vector<sf::Vector2f>& path, float speed, float hpTotal, float size, int value, sf::Color color, int points)
        : path(path), speed(speed), currentTargetIndex(1), hp(hpTotal), size(size), value(value), color(color), points(points)
    {
        shape.setRadius(size);
        shape.setFillColor(color);
        shape.setOrigin(shape.getGeometricCenter());
        shape.setPosition(path[0]);
        currentHp = hp;
        hpShape.setOutlineColor(sf::Color::Black);
        hpShape.setOutlineThickness(2.0f);
        hpShape.setFillColor(sf::Color::Green);
        hpShape.setSize({ barSize, 10.0f });
        hpShape.setOrigin(hpShape.getGeometricCenter());
        shape.setPointCount(points);


    }

    void Enemy::update(sf::Time deltaTime)
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
                distanceTravl += speed * deltaTime.asSeconds();
            }
            else
            {
                shape.setPosition(targetPosition); // Snap to the target position if close enough
                ++currentTargetIndex; // Move to the next target
            }
        }
        sf::Vector2f hpBarPos = shape.getPosition();

        hpPer = currentHp / hp;
        float newBarSize = barSize * hpPer;
        hpShape.setSize({ newBarSize, 10.0f });
        hpShape.setPosition(hpBarPos);


    }

    sf::Vector2f Enemy::currentPos() {
        return shape.getPosition();
    }

    float Enemy::distanced_Traveled() const {
        return distanceTravl;
    }

    void Enemy::updateHp(float damage, float heal) {

        currentHp += -damage + heal;
        if (hpPer > .75f) {
            hpShape.setFillColor(sf::Color::Green);
        }
        else if (hpPer > .25f) {
            hpShape.setFillColor(sf::Color::Yellow);
        }
        else {
            hpShape.setFillColor(sf::Color::Red);
        }

    }

    bool Enemy::hasReachedEnd() const
    {
        return currentTargetIndex >= path.size();
    }

    bool Enemy::dead() const {
        return currentHp <= 0;
    }

    const sf::CircleShape& Enemy::getBody() const
    {
        return shape;
    }

    const sf::RectangleShape& Enemy::getHpBar() const
    {
        return hpShape;
    }

    int Enemy::enemyValue() const {
        return value;
    }