#define _USE_MATH_DEFINES
#include <SFML/Graphics.hpp>
#include <vector>
#include "enemy.h"
#include <cmath>


    //Class Data
    Enemy::Enemy(const std::vector<sf::Vector2f>& path, float speed, float hpTotal, float size, int value, sf::Color color)
        : path(path), speed(speed), currentTargetIndex(1), hp(hpTotal), size(size), value(value), color(color)
    {
        for (int i = 0; i <= 20; ++i) {
            float angle = i * 2 * M_PI / 20;
            float x = path[0].x + size * std::cos(angle);
            float y = path[0].y + size * std::sin(angle);
            vertices.append(sf::Vertex{ {sf::Vector2f(x, y)}, color });
        }
        vertices.setPrimitiveType(sf::PrimitiveType::TriangleFan);
        this->setOrigin(path[0]);
        this->setPosition(path[0]);
        currentHp = hp;


    }

    Enemy::Enemy(const std::vector<sf::Vector2f>& path, float speed, float hpTotal, float size, int value, sf::Color color, int points)
        : path(path), speed(speed), currentTargetIndex(1), hp(hpTotal), size(size), value(value), color(color), points(points)
    {
        vertices.append(sf::Vertex{ {path[0]}, color });
        for (int i = 0; i <= points; ++i) {
            float angle = i * 2 * M_PI / points;
            float x = path[0].x + size * std::cos(angle);
            float y = path[0].y + size * std::sin(angle);
            vertices.append(sf::Vertex{ {sf::Vector2f(x, y)}, color });
        }
        vertices.setPrimitiveType(sf::PrimitiveType::TriangleFan);
        vertices.setPrimitiveType(sf::PrimitiveType::TriangleFan);
        this->setOrigin(path[0]);
        this->setPosition(path[0]);
        currentHp = hp;


    }

    void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) const { //Override

        // apply the entity's transform -- combine it with the one that was passed by the caller
        states.transform *= getTransform(); // getTransform() is defined by sf::Transformable

        // you may also override states.shader or states.blendMode if you want

        // draw the vertex array
        target.draw(vertices, states);
    }

    void Enemy::update(sf::Time deltaTime)
    {

        //Movement calculations
            if (died ==  0) { //So enemies in death animation don't move enemies don't move
                if (currentTargetIndex < path.size())
                {
                    sf::Vector2f currentPosition = this->getPosition();
                    sf::Vector2f targetPosition = path[currentTargetIndex];
                    sf::Vector2f direction = targetPosition - currentPosition;
                    float distance = abs(direction.x) + abs(direction.y);

                    if (distance > speed * deltaTime.asSeconds())
                    {
                        direction /= distance; //Remove magnitude from Direction
                        this->move(direction * speed * deltaTime.asSeconds()); //Move distance
                        distanceTravl += speed * deltaTime.asSeconds(); //Update total distance traveled
                    }
                    else
                    {
                        this->setPosition(targetPosition); // Snap to the target position if close enough
                        distanceTravl += distance; //Update total distance traveled
                        ++currentTargetIndex; // Move to the next target
                    }
                }
            }
            


        //Death animations
            if (currentHp <= 0 && died == 0) {
                died = 1;
                sf::Color newColor = sf::Color::Red;
                newColor.a = 255;
                this->updateColor(newColor);
            }
            else if (died == 1) {
                if (diedT >= sf::milliseconds(100)) {
                    died = 2;
                }
                else {
                    diedT += deltaTime;
                }
            }
    }

    void Enemy::updateColor(sf::Color newColor) {
        if ( !(newColor == color) ) {
            for (int i = 0; i < vertices.getVertexCount(); i++) {
                vertices[i].color = newColor;
                color = newColor;
            }
        }


    }

    sf::Vector2f Enemy::currentPos() const{
        return this->getPosition();
    }

    float Enemy::distanced_Traveled() const {
        return distanceTravl;
    }

    void Enemy::updateHp(float damage, float heal) {
        //Varaibles 
            sf::Color newColor = color;

        //Logic
            currentHp += -damage + heal; //Deal any damages or heals from the call
            hpPer = currentHp / hp; //Percetange of hp from total full
        
            newColor.a = 255 * hpPer; //Calculate a new Alpha value based on that Hp
            this->updateColor(newColor); //update that color

    }

    bool Enemy::hasReachedEnd() const
    {
        return currentTargetIndex >= path.size();
    }

    int Enemy::isDead() {
        return died;
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