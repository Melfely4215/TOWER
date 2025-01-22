#ifndef WAVE_H
#define WAVE_H

#include <SFML/Graphics.hpp>
#include "enemy.h"
#include <vector>



class Wave {
private:
    //Variables
    int enemyCap = 2;
    int enemyCount = 2;
    int waveId = 1;
    float counter = 0;
    float spawnDelay = 0;

public:


    //Class Data
    Wave();

    void updateInfo(sf::Time deltaTime);

    void spawnEnemies(sf::Time deltaTime, std::vector<Enemy>& enemies, int& count, const std::vector<sf::Vector2f>& waypoints);

    void enemyDied();

    int wave_Id() const;

    int enemy_Count() const;



};

#endif
