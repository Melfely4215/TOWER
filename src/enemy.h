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
    float barSize = size * 1.5; //Size of Hp Bar
    float hpPer = 1; //HP Percent
    float currentHp;
    int value;
    float distanceTravl = 0;
    int points = 20;
    sf::VertexArray vertices;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:

    //Class Data
    Enemy(const std::vector<sf::Vector2f>& path, float speed, float hpTotal, float size, int value, sf::Color color);

    Enemy(const std::vector<sf::Vector2f>& path, float speed, float hpTotal, float size, int value, sf::Color color, int points);

    void update(sf::Time deltaTime);

    sf::Vector2f currentPos();

    float distanced_Traveled() const;

    void updateHp(float damage, float heal);

    bool hasReachedEnd() const;

    bool dead() const;

    const sf::CircleShape& getBody() const;

    const sf::RectangleShape& getHpBar() const;

    int enemyValue() const;


};
#endif 


