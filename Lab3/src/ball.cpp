#include "ball.h"

Ball::Ball(glm::vec2 position, float radius, glm::vec2 velocity, glm::vec3 color)
	: Position(position), Size(radius * 2.0f, radius * 2.0f), Radius(radius), Velocity(velocity), Color(color), Stuck(true)
{
    ballTexture = Texture2D("res/textures/ball.png", true);
}

glm::vec2 Ball::Move(float dt, unsigned int windowWidth)
{
    if (!Stuck)
    {
        Position += Velocity * dt;

        if (Position.x <= 0.0f)
        {
            Velocity.x = -Velocity.x;
            Position.x = 0.0f;
        }
        else if (Position.x + Size.x >= windowWidth)
        {
            Velocity.x = -Velocity.x;
            Position.x = windowWidth - Size.x;
        }
        if (Position.y <= 0.0f)
        {
            Velocity.y = -Velocity.y;
            Position.y = 0.0f;
        }
    }

    return Position;
}

void Ball::Reset(glm::vec2 position, glm::vec2 velocity)
{
    Position = position;
    Velocity = velocity;
    Stuck = true;
}

void Ball::Draw(Renderer& renderer)
{
    renderer.Draw(ballTexture, Position, Size, 0.0f, Color);
}