#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "renderer.h"

class Paddle
{
public:
    glm::vec2 Position;
    glm::vec2 Size;
    glm::vec3 Color;

    Paddle(glm::vec2 position, glm::vec2 size, glm::vec3 color = glm::vec3(1.0f));
    void Draw(Renderer& renderer);

private:
    Texture2D paddleTexture;
};