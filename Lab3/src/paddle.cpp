#include "paddle.h"

Paddle::Paddle(glm::vec2 position, glm::vec2 size, glm::vec3 color)
	: Position(position), Size(size), Color(color)
{
	paddleTexture = Texture2D("res/textures/paddle.png", true);
}
void Paddle::Draw(Renderer& renderer)
{
	renderer.Draw(paddleTexture, Position, Size, 0.0f, Color);
}