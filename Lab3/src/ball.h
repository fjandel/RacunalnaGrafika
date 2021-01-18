#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "renderer.h"

class Ball
{
public:
    glm::vec2 Position;
    glm::vec2 Size;
    glm::vec2 Velocity;
    glm::vec3 Color;
    float Radius;
    bool Stuck;
    
    Ball(glm::vec2 position, float radius, glm::vec2 velocity = glm::vec2(0.0f, 0.0f), glm::vec3 color = glm::vec3(1.0f));
    
    glm::vec2 Move(float dt, unsigned int windowWidth);
    void Reset(glm::vec2 position, glm::vec2 velocity);
    void Draw(Renderer& renderer);

    glm::vec2 GetCenter() { return Position + Radius; }

private:
    Texture2D ballTexture;
};