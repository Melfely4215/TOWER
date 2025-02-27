#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>


class Enemy 
    : public sf::Drawable, public sf::Transformable
    {
private:
    
    sf::RectangleShape hpShape; //Object used to render HP Bar
    sf::CircleShape shape; //Object used to render body
    std::vector<sf::Vector2f> path;
    sf::Color color;
    float speed; //Speed of Enemy
    std::size_t currentTargetIndex;
    float size; //Size of Enemy
    float hp; //Starting Hp
    float hpPer = 1; //HP Percent
    float currentHp;
    int value; //Value of enemy on death
    float distanceTravl = 0; //Total distance traveled
    int points = 20; 
    sf::VertexArray vertices; //Array of drawn points used to render enemy.
    int died = 0; //Used for checking if the target has died 0 = alive, 1 = in death animation 2 = dead, remove.
    sf::Time diedT = sf::seconds(0); //Time the enemy is in the died death animation

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:

    //Class Data
    Enemy(const std::vector<sf::Vector2f>& path, float speed, float hpTotal, float size, int value, sf::Color color);

    Enemy(const std::vector<sf::Vector2f>& path, float speed, float hpTotal, float size, int value, sf::Color color, int points);

    void update(sf::Time deltaTime);

    sf::Vector2f currentPos() const;

    float distanced_Traveled() const;

    void updateColor(sf::Color newColor);

    void updateHp(float damage, float heal);

    bool hasReachedEnd() const;

    int isDead();

    const sf::CircleShape& getBody() const;

    const sf::RectangleShape& getHpBar() const;

    int enemyValue() const;


};
#endif 


