#include "Menu.h"
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;

Menu::Menu(float width, float height) {
    font.loadFromFile("STJEDISE.TTF");

    mainmenu[0].setFont(font);
    mainmenu[0].setFillColor(Color{ 255, 204, 0 });
    mainmenu[0].setString("CONTINUE");
    mainmenu[0].setCharacterSize(140);
    mainmenu[0].setPosition(Vector2f(width / 2 - 100, height / 4));

    mainmenu[1].setFont(font);
    mainmenu[1].setFillColor(Color::White);
    mainmenu[1].setString("NEW GAME");
    mainmenu[1].setCharacterSize(80);
    mainmenu[1].setPosition(Vector2f(width / 2 - 100, height / (4) + 300));

    mainmenu[2].setFont(font);
    mainmenu[2].setFillColor(Color::White);
    mainmenu[2].setString("EXIT");
    mainmenu[2].setCharacterSize(80);
    mainmenu[2].setPosition(Vector2f(width / 2 - 100, height / (4) + 400));

    selected = 0;

}

Menu::~Menu() {}

void Menu::setSelected(int n) {
    selected = n;
}

void Menu::draw(RenderWindow& window) {
    for (int i = 0; i < 3; i++) {
        window.draw(mainmenu[i]);
    }
}

void Menu::MoveDown() {
    if (selected + 1 <= 3) {
        mainmenu[selected].setFillColor(Color::White);
        selected++;
        if (selected == 3) {
            selected = 0;
        }
        mainmenu[selected].setFillColor(Color{ 255, 204, 0 });
    }
}

void Menu::MoveUp() {
    if (selected - 1 >= -1) {
        mainmenu[selected].setFillColor(Color::White);
        selected--;
        if (selected == -1) {
            selected = 2;
        }
        mainmenu[selected].setFillColor(Color{ 255, 204, 0 });
    }
}