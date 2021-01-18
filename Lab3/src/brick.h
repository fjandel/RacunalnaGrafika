#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "renderer.h"

class Brick
{
public:
    glm::vec2 Position;
    glm::vec2 Size;
    glm::vec3 Color;
    unsigned int Life;
    bool IsSolid;
    bool Destroyed;

    Brick(glm::vec2 position, glm::vec2 size, glm::vec3 color = glm::vec3(1.0f), unsigned int life = 1, bool IsSolid = false, bool Destroyed = false);
    void Draw(Renderer& renderer);
    void ReduceLife();

private:
    Texture2D brickTexture;
};