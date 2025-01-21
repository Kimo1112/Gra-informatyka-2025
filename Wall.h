#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Wall {
public:
    Wall(float x, float y, float width, float height);

    FloatRect getGlobalBounds() const;

    const RectangleShape& getShape() const;

private:
    RectangleShape wallShape;
};
