#pragma once

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "brick.h"
#include "renderer.h"


class GameLevel
{
public:
    std::vector<Brick> Bricks;

    GameLevel() {}
    GameLevel(const char* levelFile, unsigned int levelWidth, unsigned int levelHeight);

    void Draw(Renderer& renderer);
    bool IsCompleted();
    void Reset();

private:
    const char* levelFile;
    unsigned int levelWidth;
    unsigned int levelHeight;
    std::vector<std::vector<unsigned int>> tileData;

    void init();
};