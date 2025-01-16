#include "WallManager.h"

// Dodajemy �cian� do managera
void WallManager::addWall(const Wall& wall) {
    walls.push_back(wall);
}

// Zwracamy wektor �cian
const std::vector<Wall>& WallManager::getWalls() const {
    return walls;
}

// Rysowanie wszystkich �cian
void WallManager::drawWalls(sf::RenderWindow& window) const {
    for (const auto& wall : walls) {
        window.draw(wall.getShape());
    }
}