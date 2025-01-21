#include "WallManager2.h"

void WallManager2::addWall2(const Wall2& wall) {
    walls2.push_back(wall);
}

const std::vector<Wall2>& WallManager2::getWalls2() const {
    return walls2;
}

void WallManager2::drawWalls2(sf::RenderWindow& window) const {
    for (const auto& wall : walls2) {
        window.draw(wall.getShape());
    }
}