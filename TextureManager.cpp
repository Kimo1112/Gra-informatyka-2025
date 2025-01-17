#include "TextureManager.h"
#include <iostream>

using namespace std;
using namespace sf;

TextureManager::TextureManager() {}

bool TextureManager::loadAllTextures() {
    bool success = true;

    success &= loadAnimationTextures(WALKING_UP, "assets/player_walking_up_frame", 8);
    success &= loadAnimationTextures(WALKING_DOWN, "assets/player_walking_down_frame", 8);
    success &= loadAnimationTextures(WALKING_LEFT, "assets/player_walking_left_frame", 8);
    success &= loadAnimationTextures(WALKING_RIGHT, "assets/player_walking_right_frame", 8);
    success &= loadAnimationTextures(WALKING_LEFT_UP, "assets/player_walking_left_up_frame", 8);
    success &= loadAnimationTextures(WALKING_RIGHT_UP, "assets/player_walking_right_up_frame", 8);
    success &= loadAnimationTextures(WALKING_LEFT_DOWN, "assets/player_walking_left_down_frame", 8);
    success &= loadAnimationTextures(WALKING_RIGHT_DOWN, "assets/player_walking_right_down_frame", 8);
    success &= loadAnimationTextures(IDLE_UP, "assets/player_idle_up_frame", 8);
    success &= loadAnimationTextures(IDLE_DOWN, "assets/player_idle_down_frame", 8);
    success &= loadAnimationTextures(IDLE_LEFT, "assets/player_idle_left_frame", 8);
    success &= loadAnimationTextures(IDLE_RIGHT, "assets/player_idle_right_frame", 8);
    success &= loadAnimationTextures(IDLE_LEFT_UP, "assets/player_idle_left_up_frame", 8);
    success &= loadAnimationTextures(IDLE_RIGHT_UP, "assets/player_idle_right_up_frame", 8);
    success &= loadAnimationTextures(IDLE_LEFT_DOWN, "assets/player_idle_left_down_frame", 8);
    success &= loadAnimationTextures(IDLE_RIGHT_DOWN, "assets/player_idle_right_down_frame", 8);
    success &= loadAnimationTextures(ATTACKING_UP, "assets/player_attacking_up_frame", 8);

    return success;
}

bool TextureManager::loadAnimationTextures(AnimationType type, const std::string& path, int frameCount) {
    vector<Texture> textures(frameCount);
    for (int i = 0; i < frameCount; ++i) {
        if (!textures[i].loadFromFile(path + to_string(i + 1) + ".png")) {
            cerr << "Failed to load texture: " << path + to_string(i + 1) + ".png" << endl;
            return false;
        }
    }
    textureMap[type] = move(textures);
    return true;
}

const vector<Texture>& TextureManager::getTextures(AnimationType type) const {
    return textureMap.at(type);
}
