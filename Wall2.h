#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Wall2 {
public:
    Wall2(float x, float y, float width, float height);

    FloatRect getGlobalBounds() const;

    const RectangleShape& getShape() const;

private:
    RectangleShape wallShape2;
};

