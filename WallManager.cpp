#include "WallManager.h"

void WallManager::addWall(const Wall& wall) {
    walls.push_back(wall);
}

const std::vector<Wall>& WallManager::getWalls() const {
    return walls;
}

void WallManager::drawWalls(sf::RenderWindow& window) const {
    for (const auto& wall : walls) {
        window.draw(wall.getShape());
    }
}