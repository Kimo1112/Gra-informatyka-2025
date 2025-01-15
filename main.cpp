#include <SFML/Graphics.hpp>
#include <iostream>
#include "Menu.h"
#include <vector>

using namespace std;
using namespace sf;


int pagenumber = 1000;

void movement(RenderWindow& window) {
    vector<Texture> textures_walking_right(8);
    vector<Texture> textures_walking_left(8);
    vector<Texture> textures_walking_up(8);
    vector<Texture> textures_walking_down(8);
    vector<Texture> textures_walking_left_up(8);
    vector<Texture> textures_walking_left_down(8);
    vector<Texture> textures_walking_right_up(8);
    vector<Texture> textures_walking_right_down(8);
    vector<Texture> textures_idle(8);

    for (int i = 0; i < 8; ++i) {
        if (!textures_walking_right[i].loadFromFile("assets\\player_walking_right_frame" + to_string(i + 1) + ".png"));
        if (!textures_walking_left[i].loadFromFile("assets\\player_walking_left_frame" + to_string(i + 1) + ".png"));
        if (!textures_walking_up[i].loadFromFile("assets\\player_walking_up_frame" + to_string(i + 1) + ".png"));
        if (!textures_walking_down[i].loadFromFile("assets\\player_walking_down_frame" + to_string(i + 1) + ".png"));
        if (!textures_walking_left_up[i].loadFromFile("assets\\player_walking_left_up_frame" + to_string(i + 1) + ".png"));
        if (!textures_walking_left_down[i].loadFromFile("assets\\player_walking_left_down_frame" + to_string(i + 1) + ".png"));
        if (!textures_walking_right_up[i].loadFromFile("assets\\player_walking_right_up_frame" + to_string(i + 1) + ".png"));
        if (!textures_walking_right_down[i].loadFromFile("assets\\player_walking_right_down_frame" + to_string(i + 1) + ".png"));
        if (!textures_idle[i].loadFromFile("assets\\player_idle" + to_string(i + 1) + ".png"));
    }

    Sprite player;
    player.setPosition(500, 500);
    player.setScale(5, 5);

    size_t currentFrame = 0;
    Clock clock;
    float frameDuration = 0.15;

    Texture gameplay1_background;
    gameplay1_background.loadFromFile("gameplay1_background.jpeg");
    Sprite g1_bg;
    g1_bg.setTexture(gameplay1_background);

    while (window.isOpen()) {

        Event event;

        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }

        bool isMoving = false;

        if (Keyboard::isKeyPressed(Keyboard::A) && Keyboard::isKeyPressed(Keyboard::W)) {

            isMoving = true;
            player.move(-0.07, -0.07);
            if (clock.getElapsedTime().asSeconds() >= frameDuration) {
                currentFrame = (currentFrame + 1) % textures_walking_left_up.size();
                player.setTexture(textures_walking_left_up[currentFrame]);
                clock.restart();
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::A) && Keyboard::isKeyPressed(Keyboard::S)) {

            isMoving = true;
            player.move(-0.07, 0.07);
            if (clock.getElapsedTime().asSeconds() >= frameDuration) {
                currentFrame = (currentFrame + 1) % textures_walking_left_down.size();
                player.setTexture(textures_walking_left_down[currentFrame]);
                clock.restart();
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::D) && Keyboard::isKeyPressed(Keyboard::W)) {

            isMoving = true;
            player.move(0.07, -0.07);
            if (clock.getElapsedTime().asSeconds() >= frameDuration) {
                currentFrame = (currentFrame + 1) % textures_walking_right_up.size();
                player.setTexture(textures_walking_right_up[currentFrame]);
                clock.restart();
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::D) && Keyboard::isKeyPressed(Keyboard::S)) {

            isMoving = true;
            player.move(0.07, 0.07);
            if (clock.getElapsedTime().asSeconds() >= frameDuration) {
                currentFrame = (currentFrame + 1) % textures_walking_right_down.size();
                player.setTexture(textures_walking_right_down[currentFrame]);
                clock.restart();
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::D) && !Keyboard::isKeyPressed(Keyboard::W) && !Keyboard::isKeyPressed(Keyboard::S) && !Keyboard::isKeyPressed(Keyboard::A)) {

            isMoving = true;
            player.move(0.1, 0);
            if (clock.getElapsedTime().asSeconds() >= frameDuration) {
                currentFrame = (currentFrame + 1) % textures_walking_right.size();
                player.setTexture(textures_walking_right[currentFrame]);
                clock.restart();
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::A) && !Keyboard::isKeyPressed(Keyboard::W) && !Keyboard::isKeyPressed(Keyboard::S) && !Keyboard::isKeyPressed(Keyboard::D)) {

            isMoving = true;
            player.move(-0.1, 0);
            if (clock.getElapsedTime().asSeconds() >= frameDuration) {
                currentFrame = (currentFrame + 1) % textures_walking_left.size();
                player.setTexture(textures_walking_left[currentFrame]);
                clock.restart();
        }            }

        
        if (Keyboard::isKeyPressed(Keyboard::W) && !Keyboard::isKeyPressed(Keyboard::A) && !Keyboard::isKeyPressed(Keyboard::D) && !Keyboard::isKeyPressed(Keyboard::S)) {

            isMoving = true;
            player.move(0, -0.075);
            if (clock.getElapsedTime().asSeconds() >= frameDuration) {
                currentFrame = (currentFrame + 1) % textures_walking_up.size();
                player.setTexture(textures_walking_up[currentFrame]);
                clock.restart();
            }
        }
        
        if (Keyboard::isKeyPressed(Keyboard::S) && !Keyboard::isKeyPressed(Keyboard::A) && !Keyboard::isKeyPressed(Keyboard::D) && !Keyboard::isKeyPressed(Keyboard::W)) {

            isMoving = true;
            player.move(0, 0.075);
            if (clock.getElapsedTime().asSeconds() >= frameDuration) {
                currentFrame = (currentFrame + 1) % textures_walking_down.size();
                player.setTexture(textures_walking_down[currentFrame]);
                clock.restart();
            }
        }
        if (!isMoving) {
            if (clock.getElapsedTime().asSeconds() >= frameDuration) {
                currentFrame = (currentFrame + 1) % textures_idle.size();
                player.setTexture(textures_idle[currentFrame]);
                clock.restart();
            }
        }

        window.clear();
        window.draw(g1_bg);
        window.draw(player);
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
    RenderWindow window(VideoMode(1920, 1080), "GraFULLSCREEN");
    string name;

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
                movement(window);
            }
        }
    }
}