#pragma once
#include <vector>
#include "Wall.h"
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class WallManager {
public:

    void addWall(const Wall& wall);

    const vector<Wall>& getWalls() const;

    void drawWalls(RenderWindow& window) const;

private:
    vector<Wall> walls;
};

