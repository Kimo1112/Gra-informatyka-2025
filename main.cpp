#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "Menu.h"
#include "Wall.h"
#include "WallManager.h"
#include "Wall2.h"
#include "WallManager2.h"

using namespace std;
using namespace sf;

struct PlayerScore {
    string nick;
    int time;
};

struct Enemy {
    int health;
    RectangleShape shape;
    Vector2f velocity;
    Enemy(const RectangleShape& shape, int health) : shape(shape), health(health) {}
};

void spawnEnemies(vector<Enemy>& enemies, Clock& spawnClock, float& spawnInterval, Vector2f spawnPoint1, Vector2f spawnPoint2, Vector2f spawnPoint3, float acceleration) {
    if (spawnClock.getElapsedTime().asSeconds() >= spawnInterval) {

        // Losowe wybranie jednej z dwóch pozycji startowych:
        int spawnChoice = rand() % 3;
        Vector2f spawnPosition;
        if (spawnChoice == 0) {
            spawnPosition = spawnPoint1;  // Lewy bok
        }
        else if (spawnChoice == 1) {
            spawnPosition = spawnPoint2;  // Prawy bok
        }
        else {
            spawnPosition = spawnPoint3;  // Dolny bok
        }
        RectangleShape newEnemy(Vector2f(50.0f, 50.0f));  // Rozmiar przeciwnika
        newEnemy.setFillColor(Color::Red);  // Kolor przeciwnika
        newEnemy.setPosition(spawnPosition);  // Ustawienie pozycji przeciwnika

        enemies.push_back(Enemy(newEnemy, 2));  // Dodanie nowego przeciwnika z życiem 2

        // Przyspieszanie pojawiania się kolejnych przeciwników
        spawnInterval = std::max(0.5f, spawnInterval - acceleration);

        spawnClock.restart();
    }
}

void moveEnemies(vector<Enemy>& enemies, const RectangleShape& playerHitbox, float speed) {
    for (auto& enemy : enemies) {
        Vector2f direction = playerHitbox.getPosition() - enemy.shape.getPosition();
        float length = sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length != 0) {
            direction /= length;
        }

        enemy.shape.move(direction * speed);  // Przemieszczanie przeciwnika w stronę gracza
    }
}


void savePlayerScore(const string& leaderboard, const string& nickname, int time) {
    ofstream file(leaderboard, ios::app);
    if (file.is_open()) {
        file << "Name:" << nickname << ",Time:" << time << "\n";
        file.close();
    }
}

void handleDoorTransition(RectangleShape& playerHitbox, RectangleShape& door, Sprite& player, Sprite& player_shadow, vector<Texture>& walkingUpTextures, Clock& clock, float frameDuration,
    size_t& currentFrame, RectangleShape& transitionScreen, bool& transitionActive, RenderWindow& window, bool& transitionComplete, bool& isTransitioning, int& roomNumber, bool& isInAnotherRoom) {

    static bool doorTriggered = false; 
    static Clock transitionClock;     


    if (!doorTriggered && playerHitbox.getGlobalBounds().intersects(door.getGlobalBounds())) {
        doorTriggered = true;           
        door.setPosition(0, 0);            
        transitionClock.restart();        
        transitionActive = true;       
        isTransitioning = true;          
    }

    if (doorTriggered) {
        if (!transitionComplete) {
   
            if (clock.getElapsedTime().asSeconds() >= frameDuration) {
                currentFrame = (currentFrame + 1) % walkingUpTextures.size();
                player.setTexture(walkingUpTextures[currentFrame]);
                clock.restart();
            }
            player.move(0, -5); 
            player_shadow.move(0, -5);
        }

        if (transitionActive) {
            float elapsedTime = transitionClock.getElapsedTime().asSeconds();

            if (elapsedTime <= 1.0f) {
                int alpha = static_cast<int>(255 * elapsedTime); 
                transitionScreen.setFillColor(Color(0, 0, 0, alpha));
                window.draw(transitionScreen);
            }
            else {
                transitionActive = false; 
                transitionComplete = true; 
                transitionScreen.setFillColor(Color(0, 0, 0, 255));
                roomNumber = 2;
                isInAnotherRoom = true;
            }
        }
    }
}

void player_attack(vector<Enemy>& enemies, RectangleShape& attackHitbox) {
    for (auto& enemy : enemies) {
        FloatRect attackBounds = attackHitbox.getGlobalBounds();
        FloatRect enemyBounds = enemy.shape.getGlobalBounds();

        if (attackBounds.intersects(enemyBounds)) {
            enemy.health -= 1;  // Zmniejszenie zdrowia przeciwnika

            if (enemy.health <= 0) {
                // Jeśli zdrowie przeciwnika spadnie do 0, usuwamy go
                auto it = std::remove_if(enemies.begin(), enemies.end(), [&](const Enemy& e) {
                    return &e == &enemy;
                    });
                enemies.erase(it, enemies.end());
            }
        }
    }
}



void player_death(RenderWindow& window, Music& player_death_music, bool& isPlayerDead, Clock& clock, float frameDuration, Sprite& player, int lastDirection,
    const vector<Texture>& textures_dying_up, const vector<Texture>& textures_dying_down,
    const vector<Texture>& textures_dying_left, const vector<Texture>& textures_dying_right) {

    static bool animationStarted = false; 
    static size_t frameIndex = 0;      

    if (!animationStarted) {
        animationStarted = true; 
        frameIndex = 0;      
        clock.restart();    
        player_death_music.play();
    }

    const vector<Texture>* textures = nullptr;
    if (lastDirection == 1) textures = &textures_dying_up;
    else if (lastDirection == 2) textures = &textures_dying_down;
    else if (lastDirection == 3) textures = &textures_dying_left;
    else if (lastDirection == 4) textures = &textures_dying_right;

    if (textures && frameIndex < textures->size()) {
        if (clock.getElapsedTime().asSeconds() >= frameDuration) {
            player.setTexture((*textures)[frameIndex]);
            frameIndex++;                           
            clock.restart();   

            window.display();
        }
    }

    if (textures && frameIndex >= textures->size()) {
        isPlayerDead = true;
        animationStarted = false;
    }
}


void player_actions(vector<Enemy>& enemies, int& enemy_health, RectangleShape& enemy,RectangleShape& door, int& lastDirection, bool& isMoving, bool& isAttacking, bool& canAttack, bool& attackFinished, Sprite& player, Sprite& player_shadow,
    RectangleShape& playerHitbox, Clock& clock, Clock& cooldownClock,
     float& attackCooldown, float& frameDuration, size_t& currentFrame,vector<Texture>& textures_walking_up, vector<Texture>& textures_walking_down, 
    vector<Texture>& textures_walking_left, vector<Texture>& textures_walking_right, vector<Texture>& textures_walking_left_up,vector<Texture>& textures_walking_right_up,vector<Texture>& textures_walking_left_down, 
    vector<Texture>& textures_walking_right_down, vector<Texture>& textures_idle_up, vector<Texture>& textures_idle_down,
    vector<Texture>& textures_idle_left, vector<Texture>& textures_idle_right,vector<Texture>& textures_idle_left_up, vector<Texture>& textures_idle_right_up, vector<Texture>& textures_idle_left_down,
    vector<Texture>& textures_idle_right_down, vector<Texture>& textures_attacking_up, vector<Texture>& textures_attacking_down, vector<Texture>& textures_attacking_left, vector<Texture>& textures_attacking_right, 
    vector<Texture>& textures_attacking_left_up, vector<Texture>& textures_attacking_right_up, vector<Texture>& textures_attacking_left_down, vector<Texture>& textures_attacking_right_down,
    RectangleShape& attackHitboxUp, RectangleShape& attackHitboxDown, RectangleShape& attackHitboxLeft, RectangleShape& attackHitboxRight) {

    if (Keyboard::isKeyPressed(Keyboard::E)) {
        door.setPosition(0, 0);
    }
    if (Keyboard::isKeyPressed(Keyboard::Up) && canAttack && !isAttacking && !Keyboard::isKeyPressed(Keyboard::Right) && !Keyboard::isKeyPressed(Keyboard::Left) && !Keyboard::isKeyPressed(Keyboard::Down)) {
        lastDirection = 1;
        isMoving = true;
        isAttacking = true;
        attackFinished = false;
        canAttack = false;
        currentFrame = 0;
        clock.restart();
    }  
    if (Keyboard::isKeyPressed(Keyboard::Down) && canAttack && !isAttacking && !Keyboard::isKeyPressed(Keyboard::Right) && !Keyboard::isKeyPressed(Keyboard::Left) && !Keyboard::isKeyPressed(Keyboard::Up)) {
        lastDirection = 2;
        isMoving = true;
        isAttacking = true;
        attackFinished = false;
        canAttack = false;
        currentFrame = 0;
        clock.restart();
    }
    if (Keyboard::isKeyPressed(Keyboard::Left) && canAttack && !isAttacking && !Keyboard::isKeyPressed(Keyboard::Right) && !Keyboard::isKeyPressed(Keyboard::Up) && !Keyboard::isKeyPressed(Keyboard::Down)) {
        lastDirection = 3;
        isMoving = true;
        isAttacking = true;
        attackFinished = false;
        canAttack = false;
        currentFrame = 0;
        clock.restart();
    }
    if (Keyboard::isKeyPressed(Keyboard::Right) && canAttack && !isAttacking && !Keyboard::isKeyPressed(Keyboard::Up) && !Keyboard::isKeyPressed(Keyboard::Left) && !Keyboard::isKeyPressed(Keyboard::Down)) {
        lastDirection = 4;
        isMoving = true;
        isAttacking = true;
        attackFinished = false;
        canAttack = false;
        currentFrame = 0;
        clock.restart();
    }

    if (isAttacking) {
        if (lastDirection == 1) {
            if (clock.getElapsedTime().asSeconds() >= frameDuration) {
                currentFrame = (currentFrame + 1) % textures_attacking_up.size();
                if (currentFrame == 4) {
                    player_attack(enemies, attackHitboxUp);

                }
                player.setTexture(textures_attacking_up[currentFrame]);
                clock.restart();

                if (currentFrame == textures_attacking_up.size() - 1) {
                    attackFinished = true;
                    isAttacking = false;
                    isMoving = false;
                    cooldownClock.restart();
                }
            }
        }
        if (lastDirection == 2) {
            if (clock.getElapsedTime().asSeconds() >= frameDuration) {
                currentFrame = (currentFrame + 1) % textures_attacking_down.size();
                if (currentFrame == 4) {
                    player_attack(enemies, attackHitboxDown);

                }
                player.setTexture(textures_attacking_down[currentFrame]);
                clock.restart();

                if (currentFrame == textures_attacking_down.size() - 1) {
                    attackFinished = true;
                    isAttacking = false;
                    isMoving = false;
                    cooldownClock.restart();
                }
            }
        }
        if (lastDirection == 3) {
            if (clock.getElapsedTime().asSeconds() >= frameDuration) {
                currentFrame = (currentFrame + 1) % textures_attacking_left.size();
                if (currentFrame == 4) {
                    player_attack(enemies, attackHitboxLeft);

                }
                player.setTexture(textures_attacking_left[currentFrame]);
                clock.restart();

                if (currentFrame == textures_attacking_left.size() - 1) {
                    attackFinished = true;
                    isAttacking = false;
                    isMoving = false;
                    cooldownClock.restart();
                }
            }
        }
        if (lastDirection == 4) {
            if (clock.getElapsedTime().asSeconds() >= frameDuration) {
                currentFrame = (currentFrame + 1) % textures_attacking_right.size();
                if (currentFrame == 4) {
                    player_attack(enemies, attackHitboxRight);

                }
                player.setTexture(textures_attacking_right[currentFrame]);
                clock.restart();

                if (currentFrame == textures_attacking_right.size() - 1) {
                    attackFinished = true;
                    isAttacking = false;
                    isMoving = false;
                    cooldownClock.restart();
                }
            }
        }
    }

    if (!canAttack && cooldownClock.getElapsedTime().asSeconds() >= attackCooldown) {
        canAttack = true;
    }

    if (!isAttacking) {
        //UP
        if (Keyboard::isKeyPressed(Keyboard::W) && !Keyboard::isKeyPressed(Keyboard::A) && !Keyboard::isKeyPressed(Keyboard::D) && !Keyboard::isKeyPressed(Keyboard::S)) {
            lastDirection = 1;
            isMoving = true;
            player.move(0, -5);
            player_shadow.move(0, -5);
            playerHitbox.move(0, -5);
            attackHitboxUp.move(0, -5);
            attackHitboxDown.move(0, -5);
            attackHitboxLeft.move(0, -5);
            attackHitboxRight.move(0, -5);

            if (clock.getElapsedTime().asSeconds() >= frameDuration) {
                currentFrame = (currentFrame + 1) % textures_walking_up.size();
                player.setTexture(textures_walking_up[currentFrame]);
                clock.restart();
            }
        }

        //DOWN
        if (Keyboard::isKeyPressed(Keyboard::S) && !Keyboard::isKeyPressed(Keyboard::A) && !Keyboard::isKeyPressed(Keyboard::D) && !Keyboard::isKeyPressed(Keyboard::W)) {
            lastDirection = 2;
            isMoving = true;
            player.move(0, 5);
            player_shadow.move(0, 5);
            playerHitbox.move(0, 5);
            attackHitboxUp.move(0, 5);
            attackHitboxDown.move(0, 5);
            attackHitboxLeft.move(0, 5);
            attackHitboxRight.move(0, 5);

            if (clock.getElapsedTime().asSeconds() >= frameDuration) {
                currentFrame = (currentFrame + 1) % textures_walking_down.size();
                player.setTexture(textures_walking_down[currentFrame]);
                clock.restart();
            }
        }

        //LEFT
        if (Keyboard::isKeyPressed(Keyboard::A) && !Keyboard::isKeyPressed(Keyboard::W) && !Keyboard::isKeyPressed(Keyboard::S) && !Keyboard::isKeyPressed(Keyboard::D)) {
            lastDirection = 3;
            isMoving = true;
            player.move(-6, 0);
            player_shadow.move(-6, 0);
            playerHitbox.move(-6, 0);
            attackHitboxUp.move(-6, 0);
            attackHitboxDown.move(-6, 0);
            attackHitboxLeft.move(-6, 0);
            attackHitboxRight.move(-6, 0);

            if (clock.getElapsedTime().asSeconds() >= frameDuration) {
                currentFrame = (currentFrame + 1) % textures_walking_left.size();
                player.setTexture(textures_walking_left[currentFrame]);
                clock.restart();
            }
        }

        //RIGHT
        if (Keyboard::isKeyPressed(Keyboard::D) && !Keyboard::isKeyPressed(Keyboard::W) && !Keyboard::isKeyPressed(Keyboard::S) && !Keyboard::isKeyPressed(Keyboard::A)) {
            lastDirection = 4;
            isMoving = true;
            player.move(6, 0);
            player_shadow.move(6, 0);
            playerHitbox.move(6, 0);
            attackHitboxUp.move(6, 0);
            attackHitboxDown.move(6, 0);
            attackHitboxLeft.move(6, 0);
            attackHitboxRight.move(6, 0);


            if (clock.getElapsedTime().asSeconds() >= frameDuration) {
                currentFrame = (currentFrame + 1) % textures_walking_right.size();
                player.setTexture(textures_walking_right[currentFrame]);
                clock.restart();
            }
        }

        /*//LEFT UP
        if (Keyboard::isKeyPressed(Keyboard::A) && Keyboard::isKeyPressed(Keyboard::W) && !Keyboard::isKeyPressed(Keyboard::S) && !Keyboard::isKeyPressed(Keyboard::D)) {
            lastDirection = 5;
            isMoving = true;
            player.move(-5.5, -5.5);
            player_shadow.move(-5.5, -5.5);
            playerHitbox.move(-5.5, -5.5);

            if (clock.getElapsedTime().asSeconds() >= frameDuration) {
                currentFrame = (currentFrame + 1) % textures_walking_left_up.size();
                player.setTexture(textures_walking_left_up[currentFrame]);
                clock.restart();
            }
        }

        //RIGHT UP
        if (Keyboard::isKeyPressed(Keyboard::D) && Keyboard::isKeyPressed(Keyboard::W) && !Keyboard::isKeyPressed(Keyboard::S) && !Keyboard::isKeyPressed(Keyboard::A)) {
            isMoving = true;
            lastDirection = 6;
            player.move(5.5, -5.5);
            player_shadow.move(5.5, -5.5);
            playerHitbox.move(5.5, -5.5);

            if (clock.getElapsedTime().asSeconds() >= frameDuration) {
                currentFrame = (currentFrame + 1) % textures_walking_right_up.size();
                player.setTexture(textures_walking_right_up[currentFrame]);
                clock.restart();
            }
        }
        
        //LEFT DOWN
        if (Keyboard::isKeyPressed(Keyboard::A) && Keyboard::isKeyPressed(Keyboard::S) && !Keyboard::isKeyPressed(Keyboard::W) && !Keyboard::isKeyPressed(Keyboard::D)) {
            isMoving = true;
            lastDirection = 7;
            player.move(-5.5, 5.5);
            player_shadow.move(-5.5, 5.5);
            playerHitbox.move(-5.5, 5.5);

            if (clock.getElapsedTime().asSeconds() >= frameDuration) {
                currentFrame = (currentFrame + 1) % textures_walking_left_down.size();
                player.setTexture(textures_walking_left_down[currentFrame]);
                clock.restart();
            }
        }
        
        //RIGHT DOWN
        if (Keyboard::isKeyPressed(Keyboard::D) && Keyboard::isKeyPressed(Keyboard::S) && !Keyboard::isKeyPressed(Keyboard::A) && !Keyboard::isKeyPressed(Keyboard::W)) {
            isMoving = true;
            lastDirection = 8;
            player.move(5.5, 5.5);
            player_shadow.move(5.5, 5.5);
            playerHitbox.move(5.5, 5.5);

            if (clock.getElapsedTime().asSeconds() >= frameDuration) {
                currentFrame = (currentFrame + 1) % textures_walking_right_down.size();
                player.setTexture(textures_walking_right_down[currentFrame]);
                clock.restart();
            }
        }*/
    }

    if (!isMoving && !isAttacking) {
        if (lastDirection == 1) {
            if (clock.getElapsedTime().asSeconds() >= frameDuration) {
                currentFrame = (currentFrame + 1) % textures_idle_up.size();
                player.setTexture(textures_idle_up[currentFrame]);
                clock.restart();
            }
        }

        if (lastDirection == 2) {
            if (clock.getElapsedTime().asSeconds() >= frameDuration) {
                currentFrame = (currentFrame + 1) % textures_idle_down.size();
                player.setTexture(textures_idle_down[currentFrame]);
                clock.restart();
            }
        }

        if (lastDirection == 3) {
            if (clock.getElapsedTime().asSeconds() >= frameDuration) {
                currentFrame = (currentFrame + 1) % textures_idle_left.size();
                player.setTexture(textures_idle_left[currentFrame]);
                clock.restart();
            }
        }

        if (lastDirection == 4) {
            if (clock.getElapsedTime().asSeconds() >= frameDuration) {
                currentFrame = (currentFrame + 1) % textures_idle_right.size();
                player.setTexture(textures_idle_right[currentFrame]);
                clock.restart();
            }
        }
        /*
        if (lastDirection == 5) {
            if (clock.getElapsedTime().asSeconds() >= frameDuration) {
                currentFrame = (currentFrame + 1) % textures_idle_left_up.size();
                player.setTexture(textures_idle_left_up[currentFrame]);
                clock.restart();
            }
        }

        if (lastDirection == 6) {
            if (clock.getElapsedTime().asSeconds() >= frameDuration) {
                currentFrame = (currentFrame + 1) % textures_idle_right_up.size();
                player.setTexture(textures_idle_right_up[currentFrame]);
                clock.restart();
            }
        }

        if (lastDirection == 7) {
            if (clock.getElapsedTime().asSeconds() >= frameDuration) {
                currentFrame = (currentFrame + 1) % textures_idle_left_down.size();
                player.setTexture(textures_idle_left_down[currentFrame]);
                clock.restart();
            }
        }

        if (lastDirection == 8) {
            if (clock.getElapsedTime().asSeconds() >= frameDuration) {
                currentFrame = (currentFrame + 1) % textures_idle_right_down.size();
                player.setTexture(textures_idle_right_down[currentFrame]);
                clock.restart();
            }
        }*/
    }
}

void game_Floor1_Room1(Sprite& player, RectangleShape& playerHitbox, Sprite& player_shadow, RectangleShape& attackHitboxUp, RectangleShape& attackHitboxDown, RectangleShape& attackHitboxLeft,
    RectangleShape& attackHitboxRight) {

    attackHitboxUp.setPosition(858, 744);

    attackHitboxDown.setPosition(858, 923);

    attackHitboxLeft.setPosition(772, 850);

    attackHitboxRight.setPosition(914, 850);

    player.setScale(5, 5);
    player.setPosition(750, 750); //zależnei od pokoju

    playerHitbox.setPosition(850, 820); // tylko do startu gry

    player_shadow.setPosition(795, 865);
    player_shadow.setScale(5, 5);
}

void game_Floor1_Room2(Sprite& player, RectangleShape& playerHitbox, Sprite& player_shadow, RectangleShape& attackHitboxUp, RectangleShape& attackHitboxDown, RectangleShape& attackHitboxLeft,
    RectangleShape& attackHitboxRight) {

    player.setPosition(playerHitbox.getPosition() - Vector2f(100, 70));
    player_shadow.setPosition(playerHitbox.getPosition() - Vector2f(50, 50) + Vector2f(45, 115));
    attackHitboxUp.setPosition(playerHitbox.getPosition() - Vector2f(-8, 76));
    attackHitboxDown.setPosition(playerHitbox.getPosition() - Vector2f(-8, -103));
    attackHitboxLeft.setPosition(playerHitbox.getPosition() - Vector2f(78, -29));
    attackHitboxRight.setPosition(playerHitbox.getPosition() - Vector2f(-64, -29));
}

void player_Assets(Sprite& player, RectangleShape& playerHitbox, Texture& shadow, Sprite& player_shadow, RectangleShape& attackHitboxUp, RectangleShape& attackHitboxDown, RectangleShape& attackHitboxLeft,
    RectangleShape& attackHitboxRight) {

    attackHitboxUp.setSize(Vector2f(45, 74));
    attackHitboxUp.setFillColor(Color::Transparent);
    attackHitboxUp.setOutlineColor(Color::Red);
    attackHitboxUp.setOutlineThickness(2);

    attackHitboxDown.setSize(Vector2f(45, 75));
    attackHitboxDown.setFillColor(Color::Transparent);
    attackHitboxDown.setOutlineColor(Color::Red);
    attackHitboxDown.setOutlineThickness(2);

    attackHitboxLeft.setSize(Vector2f(75, 45));
    attackHitboxLeft.setFillColor(Color::Transparent);
    attackHitboxLeft.setOutlineColor(Color::Red);
    attackHitboxLeft.setOutlineThickness(2);

    attackHitboxRight.setSize(Vector2f(75, 45));
    attackHitboxRight.setFillColor(Color::Transparent);
    attackHitboxRight.setOutlineColor(Color::Red);
    attackHitboxRight.setOutlineThickness(2);

    playerHitbox.setSize(Vector2f(60, 100));
    playerHitbox.setFillColor(Color::Transparent);
    playerHitbox.setOutlineColor(Color::Red);
    playerHitbox.setOutlineThickness(2);

    shadow.loadFromFile("assets\\player_shadow.png");
    player_shadow.setTexture(shadow);
}

void objects_Floor1_Room1(RenderWindow& window, WallManager& wallManager, RectangleShape& door, Sprite& background_f1_r1, Texture& background_Floor1_Room1) {

    background_Floor1_Room1.loadFromFile("assets\\gameplay1_background.png");
    background_f1_r1.setTexture(background_Floor1_Room1);

    wallManager.addWall(Wall(480, 199, 106, 872));
    wallManager.addWall(Wall(1326, 210, 108, 860));
    wallManager.addWall(Wall(486, 961, 910, 108));
    wallManager.addWall(Wall(490, 194, 286, 124));
    wallManager.addWall(Wall(1142, 194, 285, 124));
    wallManager.addWall(Wall(673, 0, 103, 202));
    wallManager.addWall(Wall(1142, 0, 107, 209));

    door.setPosition(770, 187);
    door.setSize(Vector2f(379, 131));
    door.setFillColor(Color::Transparent);
    door.setOutlineColor(Color::Transparent);
    door.setOutlineThickness(2);
}

void objects_Floor1_Room2(RenderWindow& window, WallManager2& wallManager2, Sprite& background_f1_r2, Texture& background_Floor1_Room2) {

    background_Floor1_Room2.loadFromFile("assets\\gameplay2_background.png");
    background_f1_r2.setTexture(background_Floor1_Room2);

    wallManager2.addWall2(Wall2(1, 236, 540, 145));
    wallManager2.addWall2(Wall2(1, 662, 540, 160));
    wallManager2.addWall2(Wall2(418, 5, 123, 236));
    wallManager2.addWall2(Wall2(516, 18, 911, 140));
    wallManager2.addWall2(Wall2(1410, 21, 153, 217));
    wallManager2.addWall2(Wall2(1410, 238, 524, 145));
    wallManager2.addWall2(Wall2(1410, 668, 531, 119));
    wallManager2.addWall2(Wall2(1410, 695, 112, 250));
    wallManager2.addWall2(Wall2(420, 711, 121, 264));
    wallManager2.addWall2(Wall2(421, 882, 321, 106));
    wallManager2.addWall2(Wall2(1179, 883, 320, 101));
    wallManager2.addWall2(Wall2(644, 992, 98, 79));
    wallManager2.addWall2(Wall2(1185, 983, 101, 97));
    wallManager2.addWall2(Wall2(719, 1063, 482, 17));
    wallManager2.addWall2(Wall2(0, 211, 6, 556));
    wallManager2.addWall2(Wall2(1913, 235, 7, 528));

}

void enemy_collision(RectangleShape& playerHitboxes, Sprite& player, Sprite& player_shadow, RectangleShape& enemy,
    RectangleShape& attackHitboxUp, RectangleShape& attackHitboxDown, RectangleShape& attackHitboxLeft, RectangleShape& attackHitboxRight) {

    FloatRect playerBounds = playerHitboxes.getGlobalBounds();
    FloatRect enemy_Bounds = enemy.getGlobalBounds();

    if (playerBounds.intersects(enemy_Bounds)) {
        float overlapLeft = (playerBounds.left + playerBounds.width) - enemy_Bounds.left;
        float overlapRight = (enemy_Bounds.left + enemy_Bounds.width) - playerBounds.left;
        float overlapTop = (playerBounds.top + playerBounds.height) - enemy_Bounds.top;
        float overlapBottom = (enemy_Bounds.top + enemy_Bounds.height) - playerBounds.top;

        bool fromLeft = overlapLeft < overlapRight && overlapLeft < overlapTop && overlapLeft < overlapBottom;
        bool fromRight = overlapRight < overlapLeft && overlapRight < overlapTop && overlapRight < overlapBottom;
        bool fromTop = overlapTop < overlapBottom && overlapTop < overlapLeft && overlapTop < overlapRight;
        bool fromBottom = overlapBottom < overlapTop && overlapBottom < overlapLeft && overlapBottom < overlapRight;

        if (fromLeft) {
            playerHitboxes.setPosition(enemy_Bounds.left - playerBounds.width, playerHitboxes.getPosition().y);
            player.setPosition(enemy_Bounds.left - playerBounds.width, player.getPosition().y);
        }
        else if (fromRight) {
            playerHitboxes.setPosition(enemy_Bounds.left + enemy_Bounds.width, playerHitboxes.getPosition().y);
            player.setPosition(enemy_Bounds.left + enemy_Bounds.width, player.getPosition().y);
    
        }
        else if (fromTop) {
            playerHitboxes.setPosition(playerHitboxes.getPosition().x, enemy_Bounds.top - playerBounds.height);
            player.setPosition(player.getPosition().x, enemy_Bounds.top - playerBounds.height);
        }
        else if (fromBottom) {
            playerHitboxes.setPosition(playerHitboxes.getPosition().x, enemy_Bounds.top + enemy_Bounds.height);
            player.setPosition(player.getPosition().x, enemy_Bounds.top + enemy_Bounds.height);
        }
        player.setPosition(playerHitboxes.getPosition() - Vector2f(100, 70));
        player_shadow.setPosition(playerHitboxes.getPosition() - Vector2f(50, 50) + Vector2f(45, 115));
        attackHitboxUp.setPosition(playerHitboxes.getPosition() - Vector2f(-8, 76));
        attackHitboxDown.setPosition(playerHitboxes.getPosition() - Vector2f(-8, -103));
        attackHitboxLeft.setPosition(playerHitboxes.getPosition() - Vector2f(78, -29));
        attackHitboxRight.setPosition(playerHitboxes.getPosition() - Vector2f(-64, -29));
    }
}

void incoming_damage(RectangleShape& playerHitboxes, vector<Enemy>& enemies, int& player_health, Clock& damageCooldown, float cooldownTime) {
    FloatRect playerBounds = playerHitboxes.getGlobalBounds();

    // Sprawdzenie, czy przeciwnik wchodzi w kolizję z graczem
    for (auto& enemy : enemies) {
        FloatRect enemyBounds = enemy.shape.getGlobalBounds();

        // Jeżeli cooldown nie minął, nie wykonujemy kolejnej kolizji
        if (damageCooldown.getElapsedTime().asSeconds() < cooldownTime) {
            return;
        }

        if (playerBounds.intersects(enemyBounds)) {
            player_health -= 1;  // Zmniejszenie zdrowia gracza
            damageCooldown.restart();  // Restart cooldown
        }
    }
}
void collision_Floor1_Room1(RectangleShape& playerHitboxes, const WallManager& wallManager, Sprite& player, Sprite& player_shadow, 
    RectangleShape& attackHitboxUp, RectangleShape& attackHitboxDown, RectangleShape& attackHitboxLeft, RectangleShape& attackHitboxRight) {

    for (const auto& wall : wallManager.getWalls()) {
        FloatRect playerBounds = playerHitboxes.getGlobalBounds();
        FloatRect wallBounds = wall.getGlobalBounds();

        if (playerBounds.intersects(wallBounds)) {

            float overlapLeft = (playerBounds.left + playerBounds.width) - wallBounds.left;
            float overlapRight = (wallBounds.left + wallBounds.width) - playerBounds.left;
            float overlapTop = (playerBounds.top + playerBounds.height) - wallBounds.top;
            float overlapBottom = (wallBounds.top + wallBounds.height) - playerBounds.top;

            bool fromLeft = overlapLeft < overlapRight && overlapLeft < overlapTop && overlapLeft < overlapBottom;
            bool fromRight = overlapRight < overlapLeft && overlapRight < overlapTop && overlapRight < overlapBottom;
            bool fromTop = overlapTop < overlapBottom && overlapTop < overlapLeft && overlapTop < overlapRight;
            bool fromBottom = overlapBottom < overlapTop && overlapBottom < overlapLeft && overlapBottom < overlapRight;

            if (fromLeft) {
                playerHitboxes.setPosition(wallBounds.left - playerBounds.width, playerHitboxes.getPosition().y);
                player.setPosition(wallBounds.left - playerBounds.width, player.getPosition().y);
            }
            else if (fromRight) {
                playerHitboxes.setPosition(wallBounds.left + wallBounds.width, playerHitboxes.getPosition().y);
                player.setPosition(wallBounds.left + wallBounds.width, player.getPosition().y);
            }
            else if (fromTop) {
                playerHitboxes.setPosition(playerHitboxes.getPosition().x, wallBounds.top - playerBounds.height);
                player.setPosition(player.getPosition().x, wallBounds.top - playerBounds.height);
            }
            else if (fromBottom) {
                playerHitboxes.setPosition(playerHitboxes.getPosition().x, wallBounds.top + wallBounds.height);
                player.setPosition(player.getPosition().x, wallBounds.top + wallBounds.height);
            }

            player.setPosition(playerHitboxes.getPosition() - Vector2f(100, 70));
            player_shadow.setPosition(playerHitboxes.getPosition() - Vector2f(50, 50) + Vector2f(45, 115));
            attackHitboxUp.setPosition(playerHitboxes.getPosition() - Vector2f(-8, 76));
            attackHitboxDown.setPosition(playerHitboxes.getPosition() - Vector2f(-8, -103));
            attackHitboxLeft.setPosition(playerHitboxes.getPosition() - Vector2f(78, -29));
            attackHitboxRight.setPosition(playerHitboxes.getPosition() - Vector2f(-64, -29));
        }
    }
}

void collision_Floor1_Room2(RectangleShape& playerHitboxes, const WallManager2& wallManager2, Sprite& player, Sprite& player_shadow,
    RectangleShape& attackHitboxUp, RectangleShape& attackHitboxDown, RectangleShape& attackHitboxLeft, RectangleShape& attackHitboxRight) {

    for (const auto& wall2 : wallManager2.getWalls2()) {
        FloatRect playerBounds = playerHitboxes.getGlobalBounds();
        FloatRect wallBounds2 = wall2.getGlobalBounds();

        if (playerBounds.intersects(wallBounds2)) {

            float overlapLeft = (playerBounds.left + playerBounds.width) - wallBounds2.left;
            float overlapRight = (wallBounds2.left + wallBounds2.width) - playerBounds.left;
            float overlapTop = (playerBounds.top + playerBounds.height) - wallBounds2.top;
            float overlapBottom = (wallBounds2.top + wallBounds2.height) - playerBounds.top;

            bool fromLeft = overlapLeft < overlapRight && overlapLeft < overlapTop && overlapLeft < overlapBottom;
            bool fromRight = overlapRight < overlapLeft && overlapRight < overlapTop && overlapRight < overlapBottom;
            bool fromTop = overlapTop < overlapBottom && overlapTop < overlapLeft && overlapTop < overlapRight;
            bool fromBottom = overlapBottom < overlapTop && overlapBottom < overlapLeft && overlapBottom < overlapRight;

            if (fromLeft) {
                playerHitboxes.setPosition(wallBounds2.left - playerBounds.width, playerHitboxes.getPosition().y);
                player.setPosition(wallBounds2.left - playerBounds.width, player.getPosition().y);
            }
            else if (fromRight) {
                playerHitboxes.setPosition(wallBounds2.left + wallBounds2.width, playerHitboxes.getPosition().y);
                player.setPosition(wallBounds2.left + wallBounds2.width, player.getPosition().y);
            }
            else if (fromTop) {
                playerHitboxes.setPosition(playerHitboxes.getPosition().x, wallBounds2.top - playerBounds.height);
                player.setPosition(player.getPosition().x, wallBounds2.top - playerBounds.height);
            }
            else if (fromBottom) {
                playerHitboxes.setPosition(playerHitboxes.getPosition().x, wallBounds2.top + wallBounds2.height);
                player.setPosition(player.getPosition().x, wallBounds2.top + wallBounds2.height);
            }

            player.setPosition(playerHitboxes.getPosition() - Vector2f(100, 70));
            player_shadow.setPosition(playerHitboxes.getPosition() - Vector2f(50, 50) + Vector2f(45, 115));
            attackHitboxUp.setPosition(playerHitboxes.getPosition() - Vector2f(-8, 76));
            attackHitboxDown.setPosition(playerHitboxes.getPosition() - Vector2f(-8, -103));
            attackHitboxLeft.setPosition(playerHitboxes.getPosition() - Vector2f(78, -29));
            attackHitboxRight.setPosition(playerHitboxes.getPosition() - Vector2f(-64, -29));
        }
    }
}

void game(RenderWindow& window, int& pagenumber, string& name) {
    vector<Texture> textures_walking_up(8);

    vector<Texture> textures_walking_down(8);

    vector<Texture> textures_walking_left(8);

    vector<Texture> textures_walking_right(8);

    vector<Texture> textures_walking_left_up(8);

    vector<Texture> textures_walking_right_up(8);

    vector<Texture> textures_walking_left_down(8);

    vector<Texture> textures_walking_right_down(8);

    vector<Texture> textures_idle_up(8);

    vector<Texture> textures_idle_down(8);

    vector<Texture> textures_idle_left(8);

    vector<Texture> textures_idle_right(8);

    vector<Texture> textures_idle_left_up(8);

    vector<Texture> textures_idle_right_up(8);

    vector<Texture> textures_idle_left_down(8);

    vector<Texture> textures_idle_right_down(8);

    vector<Texture> textures_attacking_up(8);

    vector<Texture> textures_attacking_down(8);

    vector<Texture> textures_attacking_left(8);

    vector<Texture> textures_attacking_right(8);

    vector<Texture> textures_attacking_left_up(8);

    vector<Texture> textures_attacking_right_up(8);

    vector<Texture> textures_attacking_left_down(8);

    vector<Texture> textures_attacking_right_down(8);

    vector<Texture> textures_dying_up(8);

    vector<Texture> textures_dying_down(8);

    vector<Texture> textures_dying_left(8);

    vector<Texture> textures_dying_right(8);
    /*
    vector<Texture> textures_dying_left_up(8);

    vector<Texture> textures_dying_right_up(8);

    vector<Texture> textures_dying_left_down(8);

    vector<Texture> textures_dying_right_down(8);
    */

    for (int i = 0; i < 8; ++i) {
        if (!textures_walking_up[i].loadFromFile("assets\\player_walking\\player_walking_up_frame" + to_string(i + 1) + ".png"));

        if (!textures_walking_down[i].loadFromFile("assets\\player_walking\\player_walking_down_frame" + to_string(i + 1) + ".png"));

        if (!textures_walking_left[i].loadFromFile("assets\\player_walking\\player_walking_left_frame" + to_string(i + 1) + ".png"));

        if (!textures_walking_right[i].loadFromFile("assets\\player_walking\\player_walking_right_frame" + to_string(i + 1) + ".png"));

       //if (!textures_walking_left_up[i].loadFromFile("assets\\player_walking\\player_walking_left_up_frame" + to_string(i + 1) + ".png"));

       //if (!textures_walking_right_up[i].loadFromFile("assets\\player_walking\\player_walking_right_up_frame" + to_string(i + 1) + ".png"));

       //if (!textures_walking_left_down[i].loadFromFile("assets\\player_walking_left_down_frame" + to_string(i + 1) + ".png"));

       //if (!textures_walking_right_down[i].loadFromFile("assets\\player_walking_right_down_frame" + to_string(i + 1) + ".png"));

        if (!textures_idle_up[i].loadFromFile("assets\\player_idle\\player_idle_up_frame" + to_string(i + 1) + ".png"));

        if (!textures_idle_down[i].loadFromFile("assets\\player_idle\\player_idle_down_frame" + to_string(i + 1) + ".png"));

        if (!textures_idle_left[i].loadFromFile("assets\\player_idle\\player_idle_left_frame" + to_string(i + 1) + ".png"));

        if (!textures_idle_right[i].loadFromFile("assets\\player_idle\\player_idle_right_frame" + to_string(i + 1) + ".png"));

       //if (!textures_idle_left_up[i].loadFromFile("assets\\player_idle\\player_idle_left_up_frame" + to_string(i + 1) + ".png"));

       //if (!textures_idle_right_up[i].loadFromFile("assets\\player_idle\\player_idle_right_up_frame" + to_string(i + 1) + ".png"));

       //if (!textures_idle_left_down[i].loadFromFile("assets\\player_idle\\player_idle_left_frame" + to_string(i + 1) + ".png"));

       //if (!textures_idle_right_down[i].loadFromFile("assets\\player_idle\\player_idle_right_frame" + to_string(i + 1) + ".png"));

        if (!textures_attacking_up[i].loadFromFile("assets\\player_attacking\\player_attacking_up_frame" + to_string(i + 1) + ".png"));

        if (!textures_attacking_down[i].loadFromFile("assets\\player_attacking\\player_attacking_down_frame" + to_string(i + 1) + ".png"));

        if (!textures_attacking_left[i].loadFromFile("assets\\player_attacking\\player_attacking_left_frame" + to_string(i + 1) + ".png"));

        if (!textures_attacking_right[i].loadFromFile("assets\\player_attacking\\player_attacking_right_frame" + to_string(i + 1) + ".png"));

       //if (!textures_attacking_left_up[i].loadFromFile("assets\\player_attacking_left_up_frame" + to_string(i + 1) + ".png"));

       //if (!textures_attacking_right_up[i].loadFromFile("assets\\player_attacking_right_up_frame" + to_string(i + 1) + ".png"));

       //if (!textures_attacking_left_down[i].loadFromFile("assets\\player_attacking_left_down_frame" + to_string(i + 1) + ".png"));

       //if (!textures_attacking_right_down[i].loadFromFile("assets\\player_attacking_right_down_frame" + to_string(i + 1) + ".png"));

        if (!textures_dying_up[i].loadFromFile("assets\\player_dying\\player_dying_up_frame" + to_string(i + 1) + ".png"));

        if (!textures_dying_down[i].loadFromFile("assets\\player_dying\\player_dying_down_frame" + to_string(i + 1) + ".png"));

        if (!textures_dying_left[i].loadFromFile("assets\\player_dying\\player_dying_left_frame" + to_string(i + 1) + ".png"));

        if (!textures_dying_right[i].loadFromFile("assets\\player_dying\\player_dying_right_frame" + to_string(i + 1) + ".png"));

        //if (!textures_dying_left_up[i].loadFromFile("assets\\player_dying\\player_dying_left_up_frame" + to_string(i + 1) + ".png"));

        //if (!textures_dying_right_up[i].loadFromFile("assets\\player_dying\\player_dying_right_up_frame" + to_string(i + 1) + ".png"));

        //if (!textures_dying_left_down[i].loadFromFile("assets\\player_dying\\player_dying_left_down_frame" + to_string(i + 1) + ".png"));

        //if (!textures_dying_right_down[i].loadFromFile("assets\\player_dying\\player_dying_right_down_frame" + to_string(i + 1) + ".png"));
    }

    WallManager wallManager;
    WallManager2 wallManager2;
    Sprite player, player_shadow, background_f1_r1, death_screen, background_f1_r2, pause_screen, player_hearts, instructions, exit_screen;
    RectangleShape playerHitbox, door, enemy, transitionScreen(Vector2f(window.getSize().x, window.getSize().y)), attackHitboxUp, attackHitboxDown, attackHitboxLeft, attackHitboxRight,
    roomTransitionScreen(Vector2f(window.getSize().x, window.getSize().y));

    vector<Enemy> enemies;
    Clock spawnClock;   
    float spawnInterval = 5;   
    float spawnAcceleration = 0.1f;
    float enemySpeed = 1.0f;       
    Vector2f spawnPoint1(0, 540);
    Vector2f spawnPoint2(1920, 540);
    Vector2f spawnPoint3(960, 1080);

    Music player_death_music;
    player_death_music.openFromFile("assets\\player_death_music.mp3");
    player_death_music.setVolume(15);

    Texture background_Floor1_Room1, shadow, deathScreen, background_Floor1_Room2, pauseScreen, playerHearts3, playerHearts2, playerHearts1 , playerHearts0, instructionsTexture, exitScreen;
    Clock clock, damageCooldown, transitionClock, cooldownClock, gameClock;
    size_t currentFrame = 0;
    Text gameTimerText;
    Font font;

    font.loadFromFile("assets\\MedievalSharp-Regular.TTF");
    gameTimerText.setFont(font);
    gameTimerText.setCharacterSize(48);
    gameTimerText.setFillColor(Color::White);
    gameTimerText.setPosition(860, 10);
    float cooldownTime = 1.5, frameDuration = 0.1, attackCooldown = 1;
    int lastDirection = 0 , roomNumber = 1;
    int player_health = 3, enemy_health = 4;
    bool transitionActive = true, isMoving, isAttacking = false, attackFinished = false, canAttack = true, isPlayerDead = false, isDeathOver = false, isPaused = false,
        roomTransitionActive = false, isTransitioning = false, roomTransitionComplete = false, isInAnotherRoom = false, isExitOpen = false;;
    enemy.setPosition(927, 474);
    exitScreen.loadFromFile("assets\\exit_screen.png");
    exit_screen.setTexture(exitScreen);
    instructionsTexture.loadFromFile("assets\\instructions.png");
    instructions.setTexture(instructionsTexture);
    playerHearts3.loadFromFile("assets\\heart3.png");
    playerHearts2.loadFromFile("assets\\heart2.png");
    playerHearts1.loadFromFile("assets\\heart1.png");
    playerHearts0.loadFromFile("assets\\heart0.png");
    player_hearts.setTexture(playerHearts3);
    pauseScreen.loadFromFile("assets\\pause_Screen.png");
    pause_screen.setTexture(pauseScreen);
    deathScreen.loadFromFile("assets\\death_screen.png");
    death_screen.setTexture(deathScreen);
    transitionScreen.setFillColor(Color(0, 0, 0, 255));
    roomTransitionScreen.setFillColor(Color(0, 0, 0, 0));
    player.setTexture(textures_idle_right[0]); // żeby się załadował najpierw przed animacją
    player_hearts.setPosition(10, 10);
    player_hearts.setScale(5, 5);

    objects_Floor1_Room1(window, wallManager, door, background_f1_r1, background_Floor1_Room1);
    player_Assets(player, playerHitbox, shadow, player_shadow, attackHitboxUp, attackHitboxDown, attackHitboxLeft, attackHitboxRight);
    game_Floor1_Room1(player, playerHitbox, player_shadow, attackHitboxUp, attackHitboxDown, attackHitboxLeft, attackHitboxRight);
    objects_Floor1_Room2(window, wallManager2, background_f1_r2, background_Floor1_Room2);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {

            if (event.type == Event::Closed) {
                window.close();
            }
        }
        if (player_health == 3) {
            player_hearts.setTexture(playerHearts3);
        }
        if (player_health == 2) {
            player_hearts.setTexture(playerHearts2);
        }
        if (player_health == 1) {
            player_hearts.setTexture(playerHearts1);
        }
        if (player_health == 0) {
            player_hearts.setTexture(playerHearts0);
        }
        if (Keyboard::isKeyPressed(Keyboard::F1) && isPaused == false) {
            isPaused = true;
        }
        if (Keyboard::isKeyPressed(Keyboard::Space) && isPaused == true) {
            isPaused = false;
        }
        if (isPaused == true) {
            window.draw(pause_screen);
        }
        if (Keyboard::isKeyPressed(Keyboard::Escape) && isExitOpen == false) {
            isExitOpen = true;
        }
        if (isExitOpen) {
            window.draw(exit_screen);
            if (Keyboard::isKeyPressed(Keyboard::Space)) {
                isExitOpen = false;
            }
            if (Keyboard::isKeyPressed(Keyboard::Enter)) {
                pagenumber = 1000;
                return;
            }
        }
        if (isPaused == false && isExitOpen == false) {
            isMoving = false;
            if (roomNumber == 1) {
                gameClock.restart();
                window.clear();
                window.draw(background_f1_r1);
                window.draw(player_hearts);
                window.draw(player_shadow);
                window.draw(instructions);
                window.draw(player);
                window.draw(playerHitbox);
                window.draw(door);
                window.draw(enemy);
                window.draw(attackHitboxUp);
                window.draw(attackHitboxDown);
                window.draw(attackHitboxLeft);
                window.draw(attackHitboxRight);
                wallManager.drawWalls(window);
            }

            if (lastDirection == 0) {
                if (clock.getElapsedTime().asSeconds() >= frameDuration) {
                    currentFrame = (currentFrame + 1) % textures_idle_right.size();
                    player.setTexture(textures_idle_right[currentFrame]);
                    clock.restart();
                }
            }
            if (roomNumber == 1) {
                handleDoorTransition(playerHitbox, door, player, player_shadow, textures_walking_up, clock, frameDuration, currentFrame, roomTransitionScreen, roomTransitionActive, window, roomTransitionComplete,
                    isTransitioning, roomNumber, isInAnotherRoom);
                collision_Floor1_Room1(playerHitbox, wallManager, player, player_shadow, attackHitboxUp, attackHitboxDown, attackHitboxLeft, attackHitboxRight);
            }
            if (isInAnotherRoom) {
                game_Floor1_Room2(player, playerHitbox, player_shadow, attackHitboxUp, attackHitboxDown, attackHitboxLeft, attackHitboxRight);
                isInAnotherRoom = false;
            }
            if (roomNumber == 2 && player_health > 0) {
                
                window.clear();
                window.draw(background_f1_r2);
                window.draw(gameTimerText);
                window.draw(player_hearts);
                window.draw(player);
                window.draw(player_shadow);
                window.draw(attackHitboxUp);
                window.draw(attackHitboxDown);
                window.draw(attackHitboxLeft);
                window.draw(attackHitboxRight);
                window.draw(playerHitbox);
                wallManager2.drawWalls2(window);

                Time elapsed = gameClock.getElapsedTime();
                int minutes = elapsed.asSeconds() / 60;
                int seconds = static_cast<int>(elapsed.asSeconds()) % 60;

                gameTimerText.setString("Time: " + to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + to_string(seconds));

                spawnEnemies(enemies, spawnClock, spawnInterval, spawnPoint1, spawnPoint2, spawnPoint3, spawnAcceleration);

                moveEnemies(enemies, playerHitbox, enemySpeed);


                for (const auto& enemy : enemies) {
                    window.draw(enemy.shape);
                }

                incoming_damage(playerHitbox, enemies, player_health, damageCooldown, cooldownTime);


                collision_Floor1_Room2(playerHitbox, wallManager2, player, player_shadow, attackHitboxUp, attackHitboxDown, attackHitboxLeft, attackHitboxRight);
                enemy_collision(playerHitbox, player, player_shadow, enemy, attackHitboxUp, attackHitboxDown, attackHitboxLeft, attackHitboxRight);

                player_actions(enemies, enemy_health, enemy, door, lastDirection, isMoving, isAttacking, canAttack, attackFinished, player, player_shadow, playerHitbox, clock, cooldownClock, attackCooldown, frameDuration,
                    currentFrame, textures_walking_up, textures_walking_down, textures_walking_left, textures_walking_right, textures_walking_left_up, textures_walking_right_up,
                    textures_walking_left_down,
                    textures_walking_right_down, textures_idle_up, textures_idle_down, textures_idle_left, textures_idle_right, textures_idle_left_up, textures_idle_right_up,
                    textures_idle_left_down, textures_idle_right_down, textures_attacking_up, textures_attacking_down, textures_attacking_left, textures_attacking_right,
                    textures_attacking_left_up,
                    textures_attacking_right_up, textures_attacking_left_down, textures_attacking_right_down, attackHitboxUp, attackHitboxDown, attackHitboxLeft, attackHitboxRight);


            }
            if (transitionActive) {
                float elapsedTime = transitionClock.getElapsedTime().asSeconds();

                if (elapsedTime <= 1.0) {
                    int alpha = static_cast<int>(255 * (1.0 - elapsedTime));
                    transitionScreen.setFillColor(Color(0, 0, 0, alpha));
                }
                else {
                    transitionActive = false;
                }
                window.draw(transitionScreen);
            }
            else if (player_health > 0 && !isTransitioning) {

                incoming_damage(playerHitbox, enemies, player_health, damageCooldown, cooldownTime);
                enemy_collision(playerHitbox, player, player_shadow, enemy, attackHitboxUp, attackHitboxDown, attackHitboxLeft, attackHitboxRight);

                player_actions(enemies, enemy_health, enemy, door, lastDirection, isMoving, isAttacking, canAttack, attackFinished, player, player_shadow, playerHitbox, clock, cooldownClock, attackCooldown, frameDuration,
                    currentFrame, textures_walking_up, textures_walking_down, textures_walking_left, textures_walking_right, textures_walking_left_up, textures_walking_right_up,
                    textures_walking_left_down,
                    textures_walking_right_down, textures_idle_up, textures_idle_down, textures_idle_left, textures_idle_right, textures_idle_left_up, textures_idle_right_up,
                    textures_idle_left_down, textures_idle_right_down, textures_attacking_up, textures_attacking_down, textures_attacking_left, textures_attacking_right,
                    textures_attacking_left_up,
                    textures_attacking_right_up, textures_attacking_left_down, textures_attacking_right_down, attackHitboxUp, attackHitboxDown, attackHitboxLeft, attackHitboxRight);
            }
            else if (player_health == 0) {
                player_death(window, player_death_music, isPlayerDead, clock, frameDuration, player, lastDirection, textures_dying_up, textures_dying_down, textures_dying_left, textures_dying_right);
            }
            if (isPlayerDead) {
                Time finalTime = gameClock.getElapsedTime();
                int finalMinutes = finalTime.asSeconds() / 60;
                int finalSeconds = static_cast<int>(finalTime.asSeconds()) % 60;
                int totalSeconds = static_cast<int>(finalTime.asSeconds());

                this_thread::sleep_for(chrono::milliseconds(500));
                window.draw(death_screen);
                window.display();
                this_thread::sleep_for(chrono::milliseconds(1500));

                ofstream outFile("leaderboard.txt", ios::app);
                if (outFile.is_open()) {
                    outFile << "Nick:" << name << ",Time:" << totalSeconds << "\n";
                    outFile.close();
                }
                bool waitForEnter = true;
                while (waitForEnter) {
                    Event event;
                    while (window.pollEvent(event)) {
                        if (event.type == Event::Closed) {
                            window.close();
                            return;
                        }
                        if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space) {
                            pagenumber = 1000;
                            return;
                            waitForEnter = false;
                        }
                    }
                }
            }
        }
        window.display();
    }
}

vector<PlayerScore> loadLeaderboard(const string& filename) {
    vector<PlayerScore> leaderboard;
    ifstream inFile(filename);



    if (inFile.is_open()) {
        string line;
        while (getline(inFile, line)) {
            size_t nickPos = line.find("Nick:");
            size_t timePos = line.find(",Time:");

            if (nickPos != string::npos && timePos != string::npos) {
                string nick = line.substr(nickPos + 5, timePos - (nickPos + 5));
                int time = stoi(line.substr(timePos + 6));
                leaderboard.push_back({ nick, time });
            }
        }
        inFile.close();
    }

    sort(leaderboard.begin(), leaderboard.end(), [](const PlayerScore& a, const PlayerScore& b) {
        return a.time > b.time;
        });

    return leaderboard;
}

void drawLeaderboard(RenderWindow& window) {
    Font font;
    font.loadFromFile("assets\\MedievalSharp-Regular.TTF");

    vector<PlayerScore> leaderboard = loadLeaderboard("leaderboard.txt");

    Text title("Leaderboard", font, 50);
    title.setFillColor(Color::White);
    title.setPosition(100, 50);

    float yOffset = 150;
    vector<Text> leaderboardText;

    for (size_t i = 0; i < leaderboard.size() && i < 10; ++i) {
        const auto& score = leaderboard[i];

        Text nick(score.nick, font, 30);
        nick.setFillColor(Color::White);
        nick.setPosition(100, yOffset);

        int minutes = score.time / 60;
        int seconds = score.time % 60;
        Text time(to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + to_string(seconds), font, 30);
        time.setFillColor(Color::White);
        time.setPosition(300, yOffset);

        leaderboardText.push_back(nick);
        leaderboardText.push_back(time);

        yOffset += 50;
    }

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                break;
            }
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
                return;
            }
        }

        window.clear();
        window.draw(title);
        for (const auto& text : leaderboardText) {
            window.draw(text);
        }
        window.display();
    }
}

void pagename(RenderWindow& window, string& name, int& pagenumber) {

    if (!name.empty()) {
        name.clear();
    }

    Texture game_background;
    game_background.loadFromFile("game_background.png");
    Sprite g_bg;
    g_bg.setTexture(game_background);

    Font font;
    font.loadFromFile("assets\\MedievalSharp-Regular.TTF");

    Text t1;
    Text t2;

    t1.setFont(font);
    t2.setFont(font);

    t1.setString("Enter your name");

    t1.setCharacterSize(70);
    t2.setCharacterSize(70);

    t1.setPosition(10, 10);
    t2.setPosition(10, 100);

    t1.setFillColor(Color::Blue);
    t2.setFillColor(Color::Blue);

    while (window.isOpen()) {

        Event event;
        while (window.pollEvent(event)) {

            if (event.type == Event::Closed()) {
                window.close();
            }

            if (event.type == Event::TextEntered) {
                name += static_cast<char>(event.text.unicode);
            }

            if (Keyboard::isKeyPressed(Keyboard::Backspace) && name.size() > 0) {
                name.resize(name.size() - 1);
            }

            if (Keyboard::isKeyPressed(Keyboard::Return) && name.size() > 1) {
                pagenumber = 3;
                return;
            }

            if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                pagenumber = 1000;
                return;
            }

        }

        t2.setString(name);
        window.clear();
        window.draw(g_bg);
        window.draw(t1);
        window.draw(t2);
        window.display();
    }
}

int main()
{
    Music mainmenu_music;
    mainmenu_music.openFromFile("assets\\mainmenu_music.mp3");
    int pagenumber = 1000;
    mainmenu_music.setVolume(10);
    mainmenu_music.setLoop(true);
    Music game_start_music;
    game_start_music.openFromFile("assets\\game_start_music.mp3");
    game_start_music.setVolume(40);
    Menu menu(1920, 1080);
    Sprite mm_bg;
    Texture mainmenu_background;
    mainmenu_background.loadFromFile("mainmenu_background.png");
    mm_bg.setTexture(mainmenu_background);
    RenderWindow window(VideoMode(1920, 1080), "GAME");
    string name;
    window.setFramerateLimit(60);

    mainMenu:
    while (true) {
        if (pagenumber == 1000) {
            mainmenu_music.play();
            while (window.isOpen()) {
                Event event;
                while (window.pollEvent(event)) {

                    if (event.type == Event::Closed) {
                        window.close();
                        break;
                    }

                    if (Keyboard::isKeyPressed(Keyboard::Up)) {
                        menu.MoveUp();
                    }

                    if (Keyboard::isKeyPressed(Keyboard::Down)) {
                        menu.MoveDown();
                    }

                    if (Keyboard::isKeyPressed(Keyboard::Return)) {

                        if (menu.pressed() == 0) {
                            pagenumber = 0;
                        }

                        if (menu.pressed() == 1) {
                            pagenumber = 1;
                        }

                        if (menu.pressed() == 2) {
                            pagenumber = -1;
                        }

                    }
                }

                window.clear();
                if (pagenumber != 1000) {
                    break;  
                }

                window.draw(mm_bg);
                menu.draw(window);
                window.display();
            }

            if (pagenumber == -1) {
                window.close();
                exit(0);
            }
            if (pagenumber == 1) {
                drawLeaderboard(window);
                pagenumber = 1000;
            }
            if (pagenumber == 0) {
                pagename(window, name, pagenumber);
            }

            if (pagenumber == 3) {
                mainmenu_music.stop();
                game_start_music.play();
                game(window, pagenumber , name);
            }
        }
    }
}