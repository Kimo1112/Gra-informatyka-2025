#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include "Menu.h"
#include "Wall.h"
#include "WallManager.h"

using namespace std;
using namespace sf;

int pagenumber = 1000;

void player_death() {
        
}

void player_actions(RectangleShape& door, int& lastDirection, bool& isMoving, bool& isAttacking, bool& canAttack, bool& attackFinished, Sprite& player, Sprite& player_shadow,
    RectangleShape& playerHitbox, Clock& clock, Clock& cooldownClock,
     float& attackCooldown, float& frameDuration, size_t& currentFrame,vector<Texture>& textures_walking_up, vector<Texture>& textures_walking_down, 
    vector<Texture>& textures_walking_left, vector<Texture>& textures_walking_right, vector<Texture>& textures_walking_left_up,vector<Texture>& textures_walking_right_up,vector<Texture>& textures_walking_left_down, 
    vector<Texture>& textures_walking_right_down, vector<Texture>& textures_idle_up, vector<Texture>& textures_idle_down,
    vector<Texture>& textures_idle_left, vector<Texture>& textures_idle_right,vector<Texture>& textures_idle_left_up, vector<Texture>& textures_idle_right_up, vector<Texture>& textures_idle_left_down,
    vector<Texture>& textures_idle_right_down, vector<Texture>& textures_attacking_up, vector<Texture>& textures_attacking_down, vector<Texture>& textures_attacking_left, vector<Texture>& textures_attacking_right, 
    vector<Texture>& textures_attacking_left_up, vector<Texture>& textures_attacking_right_up, vector<Texture>& textures_attacking_left_down, vector<Texture>& textures_attacking_right_down) {

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

void game_Floor1_Room1(Sprite& player, RectangleShape& playerHitbox, Sprite& player_shadow) {
    player.setScale(5, 5);
    player.setPosition(750, 750); //zależne od pokoju

    playerHitbox.setPosition(850, 820); // tylko do startu gry

    player_shadow.setPosition(795, 865);
    player_shadow.setScale(5, 5);
}

void player_Assets(Sprite& player, RectangleShape& playerHitbox, Texture& shadow, Sprite& player_shadow) {

    playerHitbox.setSize(Vector2f(60, 100));
    playerHitbox.setFillColor(Color::Transparent);
    playerHitbox.setOutlineColor(Color::Red);
    playerHitbox.setOutlineThickness(2);

    shadow.loadFromFile("assets\\player_shadow.png");
    player_shadow.setTexture(shadow);
}

void objects_Floor1_Room1(RenderWindow& window, WallManager& wallManager, RectangleShape& door, RectangleShape& enemy, Sprite& background_f1_r1, Texture& background_Floor1_Room1) {

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
    door.setOutlineColor(Color::Red);
    door.setOutlineThickness(2);

    enemy.setPosition(1150, 788);
    enemy.setSize(Vector2f(100, 100));
    enemy.setFillColor(Color::Transparent);
    enemy.setOutlineColor(Color::Cyan);
    enemy.setOutlineThickness(2);
}

void enemy_collision(RectangleShape& playerHitboxes, Sprite& player, Sprite& player_shadow, RectangleShape& enemy) {
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
    }
}

void incoming_damage(RectangleShape& playerHitboxes, RectangleShape& enemy, int& player_health, Clock& damageCooldown, float cooldownTime) {
    FloatRect playerBounds = playerHitboxes.getGlobalBounds();
    FloatRect enemy_Bounds = enemy.getGlobalBounds();
    if (damageCooldown.getElapsedTime().asSeconds() < cooldownTime) {
        return;
    }
    if (playerBounds.intersects(enemy_Bounds)) {
        player_health -= 1;
        damageCooldown.restart();
        cout << player_health << endl;

    }
}

void collision_Floor1_Room1(RectangleShape& playerHitboxes, const WallManager& wallManager, Sprite& player, Sprite& player_shadow) {
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
        }
    }
}

void game(RenderWindow& window) {
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
    }

    WallManager wallManager;
    Sprite player, player_shadow, background_f1_r1;
    RectangleShape playerHitbox, door, enemy, transitionScreen(Vector2f(window.getSize().x, window.getSize().y));
    Texture background_Floor1_Room1, shadow;
    Clock clock, damageCooldown, transitionClock, cooldownClock;
    size_t currentFrame = 0;

    float cooldownTime = 1.5, frameDuration = 0.1, attackCooldown = 2;
    int lastDirection = 0;
    int player_health = 3; //wrzuć do pliku
    bool transitionActive = true, isMoving, isAttacking = false, attackFinished = false, canAttack = true;

    transitionScreen.setFillColor(Color(0, 0, 0, 255));
    player.setTexture(textures_idle_right[0]); // żeby się załadował najpierw przed animacją

    objects_Floor1_Room1(window, wallManager, door, enemy, background_f1_r1, background_Floor1_Room1);
    player_Assets(player, playerHitbox, shadow, player_shadow);
    game_Floor1_Room1(player, playerHitbox, player_shadow);


    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {

            if (event.type == Event::Closed) {
                window.close();
            }
        }

        isMoving = false;

        window.clear();
        window.draw(background_f1_r1);
        window.draw(player_shadow);
        window.draw(player);
        window.draw(playerHitbox);
        window.draw(door);
        window.draw(enemy);
        wallManager.drawWalls(window);

        if (lastDirection == 0) {
            if (clock.getElapsedTime().asSeconds() >= frameDuration) {
                currentFrame = (currentFrame + 1) % textures_idle_right.size();
                player.setTexture(textures_idle_right[currentFrame]);
                clock.restart();
            }
        }

        collision_Floor1_Room1(playerHitbox, wallManager, player, player_shadow);
        incoming_damage(playerHitbox, enemy, player_health, damageCooldown, cooldownTime);
        enemy_collision(playerHitbox, player, player_shadow, enemy);

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
        else if (player_health>0){
            player_actions(door, lastDirection, isMoving, isAttacking, canAttack, attackFinished, player, player_shadow, playerHitbox, clock, cooldownClock, attackCooldown, frameDuration,
                currentFrame, textures_walking_up,textures_walking_down, textures_walking_left, textures_walking_right, textures_walking_left_up, textures_walking_right_up, 
                textures_walking_left_down,
                textures_walking_right_down, textures_idle_up, textures_idle_down, textures_idle_left, textures_idle_right, textures_idle_left_up, textures_idle_right_up,
                textures_idle_left_down, textures_idle_right_down, textures_attacking_up, textures_attacking_down, textures_attacking_left, textures_attacking_right,
                textures_attacking_left_up, 
                textures_attacking_right_up, textures_attacking_left_down, textures_attacking_right_down);
        }
        else {
            player_death;
        }
        window.display();
    }
}

void pagename(RenderWindow& window, string& name) {

    if (!name.empty()) {
        name.clear();
    }

    Texture game_background;
    game_background.loadFromFile("game_background.png");
    Sprite g_bg;
    g_bg.setTexture(game_background);

    Font font;
    font.loadFromFile("STJEDISE.TTF");

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
    Menu menu(1920, 1080);
    Sprite mm_bg;
    Texture mainmenu_background;
    mainmenu_background.loadFromFile("mainmenu_background.png");
    mm_bg.setTexture(mainmenu_background);
    RenderWindow window(VideoMode(1920, 1080), "GRA");
    string name;
    window.setFramerateLimit(60);

    while (true) {
        if (pagenumber == 1000) {
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
                break;
            }

            if (pagenumber == 0) {
                pagename(window, name);
            }

            if (pagenumber == 3) {
                game(window);
            }
        }
    }
}