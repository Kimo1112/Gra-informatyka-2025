#include "Wall2.h"

Wall2::Wall2(float x, float y, float width, float height) {
    wallShape2.setPosition(x, y);
    wallShape2.setSize(Vector2f(width, height));
    wallShape2.setFillColor(Color::Transparent);
    wallShape2.setOutlineColor(Color::Red);
    wallShape2.setOutlineThickness(2);
}

FloatRect Wall2::getGlobalBounds() const {
    return wallShape2.getGlobalBounds();
}

const RectangleShape& Wall2::getShape() const {
    return wallShape2;
}