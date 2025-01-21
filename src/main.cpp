#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include <ctime>



class Enemy
{
public:

    //Class Data
    Enemy(const std::vector<sf::Vector2f>& path, float speed, float hpTotal, float size, int value, sf::Color color)
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
        
        
    }

    void update(sf::Time deltaTime)
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

    sf::Vector2f currentPos() {
        return shape.getPosition();
    }

    float distanced_Traveled() {
        return distanceTravl;
    }

    void updateHp(float damage, float heal) {

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

    bool hasReachedEnd() const
    {
        return currentTargetIndex >= path.size();
    }
    
    bool dead() const {
        return currentHp <= 0;
    }
    
    const sf::CircleShape& getBody() const
    {
        return shape;
    }

    const sf::RectangleShape& getHpBar() const
    {
        return hpShape;
    }

    int enemyValue() const {
        return value;
    }

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
};

class Turret {

public:
    Turret(sf::Vector2f location, int damage, float shotsPerSec, int pointsCount, sf::Color hullColor, sf::Color shotColor, float aoeSize, int cost, float range)
        : location(location), damage(damage), fireDelay(1/shotsPerSec), pointsCount(pointsCount), hullColor(hullColor), shotColor(shotColor), cost(cost), aoeSize(aoeSize), range(range) {
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

    Turret(sf::Vector2f location, int damage, float shotsPerSec, int pointsCount, sf::Color hullColor, sf::Color shotColor, float aoeSize, int cost, float range, int ammo, float reloadDelay )
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

    void shoot(sf::Time deltaTime, std::vector<Enemy>& enemies){
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
            if (currentBestDist <= range && !(bestEnemy == -1) ) {
                lastShot = 0;
                sf::Vector2f enemyLocation = enemies[bestEnemy].currentPos();
                shot[1].position = enemyLocation;
                shotAoe.setPosition(enemyLocation);
                if (aoeSize > 0) {
                    
                    static std::array<sf::Vector2f, 2> attackArea;

                    attackArea[0] = sf::Vector2f(enemyLocation.x - aoeSize*1.5, enemyLocation.y - aoeSize*1.5);
                    attackArea[1] = sf::Vector2f(enemyLocation.x + aoeSize*1.5, enemyLocation.y + aoeSize*1.5);

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

    const sf::CircleShape& getHull() const {
        return hull;
    }

    const sf::CircleShape& getRange() const {
        return rangeDraw;
    }
    
    const sf::VertexArray& getShot() const {
        return shot;
    }

    const sf::CircleShape& getShotAoe() const {
        return shotAoe;
    }

    const float& getShotTime() const {
        return lastShot;
    }

private:
    //Turret Stats
        int damage;
        float fireDelay;
        float lastShot = -1;
        int pointsCount;
        int ammo = -1;
        float reloadDelay = 0;
        int cost;
        float aoeSize;
        float range;
        
    //Rendering Data
        sf::CircleShape rangeDraw;
        sf::CircleShape hull;
        sf::Color hullColor;
        sf::VertexArray shot;
        sf::Color shotColor;
        sf::Vector2f location;
        sf::CircleShape shotAoe; 

};


class Wave {
public:

    
    //Class Data
    Wave() {

    }

    void updateInfo(sf::Time deltaTime) {
        
        if (enemyCount == 0) {
            if (counter >= 2) {
                waveId++;
                enemyCap = enemyCap + waveId *.5;
                enemyCount = enemyCap;
                counter = 0;
            }
            else {
                counter += deltaTime.asSeconds();
            }

        }

    }
    void spawnEnemies(sf::Time deltaTime, std::vector<Enemy>& enemies, int& count, const std::vector<sf::Vector2f>& waypoints) {
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

    void enemyDied() {
        enemyCount = enemyCount - 1;
    }

    int wave_Id() const {
        return waveId;
    }
    int enemy_Count() const {
        return enemyCount;
    }

private:
    //Variables
    int enemyCap = 2;
    int enemyCount = 2;
    int waveId = 1;
    float counter = 0;
    float spawnDelay = 0;

};

void attackHandle(bool& drawAttack, sf::Vector2i& attackLocationInt, sf::CircleShape& attackCircle, std::vector<Enemy>& enemies) {
    //variables
        static std::array<sf::Vector2f, 2> attackArea;
        drawAttack = true;
        sf::Vector2f attackLocation = static_cast<sf::Vector2f>(attackLocationInt);
        sf::Vector2f enemyPos;
    //Logic
       
        attackCircle.setPosition(attackLocation);
    
        attackArea[0] = sf::Vector2f(attackCircle.getPosition().x - attackCircle.getRadius()*1.5, attackCircle.getPosition().y - attackCircle.getRadius()*1.5);
        attackArea[1] = sf::Vector2f(attackCircle.getPosition().x + attackCircle.getRadius()*1.5, attackCircle.getPosition().y + attackCircle.getRadius()*1.5);

        for (auto it = enemies.begin(); it != enemies.end();) {
            enemyPos = it->currentPos();
            if (enemyPos.x >= attackArea[0].x && enemyPos.y >= attackArea[0].y && enemyPos.x <= attackArea[1].x && enemyPos.y <= attackArea[1].y) {
                it->updateHp(25, 0);
                it++;
            }
            else {
                it++;
            }
        }
}

int main()
{
    std::srand(time(0));
    Wave waves;
    sf::RenderWindow window(sf::VideoMode({ 1920, 1080 }), "Tower Defense Game");
    window.setVerticalSyncEnabled(true); // Enable V-Sync
    //window.setFramerateLimit(60);
    sf::Vector2 windowSize = window.getSize();

    // Define waypoints for the path
    std::vector<sf::Vector2f> waypoints = {
        {0.f,  windowSize.y * 0.10f },
        {windowSize.x * 0.90f, windowSize.y * 0.10f},
        {windowSize.x * 0.90f, windowSize.y * 0.30f},
        {windowSize.x * 0.10f, windowSize.y * 0.30f},
        {windowSize.x * 0.10f, windowSize.y * 0.80f},
        {windowSize.x * 1.0f, windowSize.y * 0.80f},
        
    };

    // Set initial position of the enemy at the start of the path
    
    // Create a vertex array for the path
    sf::VertexArray line(sf::PrimitiveType::LineStrip, waypoints.size());
    for (std::size_t i = 0; i < waypoints.size(); ++i)
    {
        line[i].position = waypoints[i];
        line[i].color = sf::Color::Red;
    }

    // Load a font
    sf::Font font;
    if (!font.openFromFile("Roboto-Regular.ttf"))
    {
        std::cerr << "Could not load font" << std::endl;
    }

    
    // Create a text object to display the framerate
    sf::Text framerateText(font);
    framerateText.setCharacterSize(24);
    framerateText.setFillColor(sf::Color::White);
    framerateText.setPosition({0.0f, windowSize.y * .96f});
    //Create an object to display enemy count and wave number
    sf::Text waveInfo(font);
    waveInfo.setCharacterSize(32);
    waveInfo.setFillColor(sf::Color::White);
    waveInfo.setPosition({ 0.0f, 20.0f });
    //Create attack shape
        sf::CircleShape attackCircle;
        attackCircle.setRadius(50.0f);
        attackCircle.setOutlineThickness(2.0f);
        attackCircle.setOutlineColor(sf::Color::Red);
        attackCircle.setFillColor(sf::Color::Transparent);
        attackCircle.setOrigin(attackCircle.getGeometricCenter());
        bool drawAttack = false;
        sf::Vector2i attackLocation;

    // Create a list of enemies
    std::vector<Enemy> enemies;
    //Create a list of Turrets
    std::vector<Turret> turrets;
    // Variables to control which half to update
    //Total Enemy Count
    int count = 0;
    //Spawn Delay
    float spawn = 0;
    bool buffer = false;
    

    // Main game loop
    sf::Clock clock; // to track time for consistent movement

    while (window.isOpen())
    {
        // Time since last frame
        sf::Time deltaTime = clock.restart();
        
        // Add an enemy to the list
        
        

        // Process events
        while (auto event = window.pollEvent())
        {
            if (event.has_value())
            {
                if (event->is<sf::Event::Closed>())
                    window.close();
                
                if (const auto& mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouseEvent->button == sf::Mouse::Button::Left) {
                        attackLocation = mouseEvent->position;
                        attackHandle(drawAttack, attackLocation, attackCircle, enemies);
                        
                    }
                }

                
                
            }

           
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1) && buffer == false) {
            sf::Vector2f mousePos;
            mousePos.x = sf::Mouse::getPosition(window).x;
            mousePos.y = sf::Mouse::getPosition(window).y;
            turrets.push_back(Turret(mousePos, 25, 5.0f, 3, sf::Color::Red, sf::Color::Blue, 0.0f, 1, 200.0f));
            buffer = true;
        }
        else if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1))) {
            buffer = false;
        }

        for (auto turret = turrets.begin(); turret != turrets.end();) {
            turret->shoot(deltaTime, enemies);
            turret++;
        }
        
        // Check if any enemy has reached the end of the path and remove them
        for (auto it = enemies.begin(); it != enemies.end();)
        {
            it->update(deltaTime);
            
            if (it->dead() ) {
                it = enemies.erase(it);
                waves.enemyDied();
                --count;
            }
            
            else if (it->hasReachedEnd())
            {
                it = enemies.erase(it); // Remove the enemy from the list
                waves.enemyDied();
                --count;
            }
            else
            {
                ++it; // Move to the next enemy
            }
        }

        

        waves.spawnEnemies(deltaTime, enemies, count, waypoints);
        waves.updateInfo(deltaTime);
        // Update UI text
        float fps = 1.f / deltaTime.asSeconds();
        framerateText.setString("FPS: " + std::to_string(static_cast<int>(fps)) );
        waveInfo.setString("Wave: " + std::to_string(waves.wave_Id()) + " Enemies Left: " + std::to_string(waves.enemy_Count()));

        // Clear the screen
        window.clear();

        // Draw the path
        window.draw(line);

        // Draw the framerate text
        window.draw(framerateText);
        window.draw(waveInfo);
        if (drawAttack == true) {
            window.draw(attackCircle);
            drawAttack = false;
        }

        // Draw the enemy
        for (const auto& enemy : enemies)
        {
            window.draw(enemy.getBody());
            window.draw(enemy.getHpBar());
        }
        //Draw Turrets
        for (const auto& turret : turrets) {
            window.draw(turret.getHull());
            window.draw(turret.getRange());
            if (turret.getShotTime() <= .05) {
                window.draw(turret.getShot());
                window.draw(turret.getShotAoe());
            }
            
        }

        // Display what was drawn
        window.display();
    }

    return 0;
}
