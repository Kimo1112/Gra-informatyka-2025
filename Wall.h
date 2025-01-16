#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Wall {
public:
    // Konstruktor klasy Wall, kt�ry przyjmuje pozycj� (x, y) oraz wymiary (szeroko��, wysoko��)
    Wall(float x, float y, float width, float height);

    // Metoda do pobrania granic (global bounds) �ciany
    FloatRect getGlobalBounds() const;

    // Metoda do pobrania kszta�tu �ciany (RectangleShape) 
    const RectangleShape& getShape() const;

private:
    RectangleShape wallShape;  // Kszta�t �ciany
};
