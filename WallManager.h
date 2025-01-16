#pragma once
#include <vector>
#include "Wall.h"
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class WallManager {
public:
    // Dodajemy œcianê do managera
    void addWall(const Wall& wall);

    // Zwracamy wektor œcian
    const vector<Wall>& getWalls() const;

    // Rysowanie wszystkich œcian
    void drawWalls(RenderWindow& window) const;

private:
    vector<Wall> walls;  // Przechowujemy wszystkie œciany
};

