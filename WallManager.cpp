#include "WallManager.h"

// Dodajemy œcianê do managera
void WallManager::addWall(const Wall& wall) {
    walls.push_back(wall);
}

// Zwracamy wektor œcian
const std::vector<Wall>& WallManager::getWalls() const {
    return walls;
}

// Rysowanie wszystkich œcian
void WallManager::drawWalls(sf::RenderWindow& window) const {
    for (const auto& wall : walls) {
        window.draw(wall.getShape());
    }
}