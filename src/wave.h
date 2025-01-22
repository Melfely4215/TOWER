#ifndef WAVE_H
#define WAVE_H

#include <SFML/Graphics.hpp>
#include "enemy.h"
#include <vector>
#include "types.h"



class Wave {
private:
    //Variables
        int enemyCap = 2;
        int enemyCount = 2;
        int waveId = 1;
        float counter = 0;
        float spawnDelay = 0;
        int money = 25;

    //Enemies
    

public:


    //Class Data
    Wave();

    void updateInfo(sf::Time deltaTime);

    void spawnEnemies(sf::Time deltaTime, std::vector<Enemy>& enemies, int& count, const std::vector<sf::Vector2f>& waypoints);

    void enemyDied(int reward);

    void spendMoney(int spentAmount);

    const int&  wave_Id() const;

    const int& enemy_Count() const;

    const int& returnMoney() const;

};

#endif
