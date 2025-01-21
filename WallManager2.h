#pragma once
#include <vector>
#include "Wall2.h"
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class WallManager2 {
public:

    void addWall2(const Wall2& wall);

    const vector<Wall2>& getWalls2() const;

    void drawWalls2(RenderWindow& window) const;

private:
    vector<Wall2> walls2;
};