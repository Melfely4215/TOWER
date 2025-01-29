#include <SFML/Graphics.hpp>
#include "enemy.h"
#include <cmath>
#include <vector>
#include "wave.h"
#include "types.h"

    //Class Data
    Wave::Wave() {

    }

    void Wave::updateInfo(sf::Time deltaTime) {

        if (enemyCount == 0) {
            if (counter >= 2) {
                waveId++;
                enemyCap = enemyCap + waveId * .5;
                enemyCount = enemyCap;
                counter = 0;
            }
            else {
                counter += deltaTime.asSeconds();
            }

        }

    }
    void Wave::spawnEnemies(sf::Time deltaTime, std::vector<Enemy>& enemies, int& count, const std::vector<sf::Vector2f>& waypoints) {
        //Enemies
            static Basic_Enemy basicEnemy = { waypoints };
            static Fast_Enemy fastEnemy = { waypoints };


        int enemyType = (rand() % 2) + 1;
        if (!(count >= enemyCount) && spawnDelay <= 0) {
            if (enemyType == 2) {
                enemies.emplace_back(fastEnemy); //Fast Guy
            }
            else {
                enemies.emplace_back(basicEnemy); //Base Enemy
            }

            count++;
            spawnDelay = 1 / sqrt(waveId);
        }
        else {
            spawnDelay -= static_cast<float>(deltaTime.asSeconds());
        }
    }

    void Wave::debugEnemies(sf::Time deltaTime, std::vector<Enemy>& enemies, int& count, const std::vector<sf::Vector2f>& waypoints) {
        health = 1000;
        enemyCount = 1000;
        money = 1000;
        if (!(count >= enemyCount) && spawnDelay <= 0) {
            enemies.emplace_back(waypoints, 100, 10000, 25, 1, sf::Color::White);
            count++;
            spawnDelay = 0;
        }
        else {
            spawnDelay -= static_cast<float>(deltaTime.asSeconds());
        }
    }

    void Wave::enemyWon(int damage) {
        enemyCount -= 1;
        health -= damage;
    }

    void Wave::enemyDied(int reward) {
        enemyCount = enemyCount - 1;
        money += reward;

    }

    void Wave::spendMoney(int spentAmount) {
        money = money - spentAmount;
    }

    const int& Wave::wave_Id() const { return waveId; }

    const int& Wave::enemy_Count() const { return enemyCount; }

    const int& Wave::returnMoney() const { return money; }

    const int& Wave::returnHealth() const { return health; }