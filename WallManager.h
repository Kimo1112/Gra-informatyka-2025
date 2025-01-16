#pragma once
#include <vector>
#include "Wall.h"
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class WallManager {
public:
    // Dodajemy �cian� do managera
    void addWall(const Wall& wall);

    // Zwracamy wektor �cian
    const vector<Wall>& getWalls() const;

    // Rysowanie wszystkich �cian
    void drawWalls(RenderWindow& window) const;

private:
    vector<Wall> walls;  // Przechowujemy wszystkie �ciany
};

