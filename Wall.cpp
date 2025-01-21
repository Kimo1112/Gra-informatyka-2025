#include "Wall.h"

Wall::Wall(float x, float y, float width, float height) {
    wallShape.setPosition(x, y);
    wallShape.setSize(Vector2f(width, height));
    wallShape.setFillColor(Color::Transparent);
    wallShape.setOutlineColor(Color::Transparent);
    wallShape.setOutlineThickness(2);
}

FloatRect Wall::getGlobalBounds() const {
    return wallShape.getGlobalBounds();
}

const RectangleShape& Wall::getShape() const {
    return wallShape;
}