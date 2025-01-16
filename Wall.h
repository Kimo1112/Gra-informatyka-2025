#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Wall {
public:
    // Konstruktor klasy Wall, który przyjmuje pozycjê (x, y) oraz wymiary (szerokoœæ, wysokoœæ)
    Wall(float x, float y, float width, float height);

    // Metoda do pobrania granic (global bounds) œciany
    FloatRect getGlobalBounds() const;

    // Metoda do pobrania kszta³tu œciany (RectangleShape) 
    const RectangleShape& getShape() const;

private:
    RectangleShape wallShape;  // Kszta³t œciany
};
