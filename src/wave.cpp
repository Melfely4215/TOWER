#include <SFML/Graphics.hpp>
#include "enemy.h"
#include <cmath>
#include <vector>
#include "wave.h"

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
        int enemyType = (rand() % 2) + 1;
        if (!(count >= enemyCount) && spawnDelay <= 0) {
            if (enemyType == 2) {
                enemies.emplace_back(waypoints, 300, 25, 15, 1, sf::Color::Blue); //Fast Guy
            }
            else {
                enemies.emplace_back(waypoints, 100, 100, 25, 1, sf::Color::White); //Base Enemy
            }

            count++;
            spawnDelay = 1 / sqrt(waveId);
        }
        else {
            spawnDelay -= static_cast<float>(deltaTime.asSeconds());
        }
    }

    void Wave::enemyDied() {
        enemyCount = enemyCount - 1;
    }

    int Wave::wave_Id() const {
        return waveId;
    }
    int Wave::enemy_Count() const {
        return enemyCount;
    }