#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;
using namespace sf;

class TextureManager {
public:
    enum AnimationType {
        WALKING_UP,
        WALKING_DOWN,
        WALKING_LEFT,
        WALKING_RIGHT,
        WALKING_LEFT_UP,
        WALKING_RIGHT_UP,
        WALKING_LEFT_DOWN,
        WALKING_RIGHT_DOWN,
        IDLE_UP,
        IDLE_DOWN,
        IDLE_LEFT,
        IDLE_RIGHT,
        IDLE_LEFT_UP,
        IDLE_RIGHT_UP,
        IDLE_LEFT_DOWN,
        IDLE_RIGHT_DOWN,
        ATTACKING_UP
    };

    TextureManager();
    bool loadAllTextures();
    const vector<Texture>& getTextures(AnimationType type) const;

private:
    unordered_map<AnimationType, vector<Texture>> textureMap;
    bool loadAnimationTextures(AnimationType type, const string& path, int frameCount);
};



