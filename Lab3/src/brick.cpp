#include "brick.h"

Brick::Brick(glm::vec2 position, glm::vec2 size, glm::vec3 color, unsigned int life, bool isSolid, bool destroyed)
	: Position(position), Size(size), Color(color), Life(life), IsSolid(isSolid), Destroyed(destroyed) 
{
	if (life == 3)
		brickTexture = Texture2D("res/textures/block_solid.png", false);
	else
		brickTexture = Texture2D("res/textures/block.png", false);
}
void Brick::Draw(Renderer& renderer)
{
	renderer.Draw(brickTexture, Position, Size, 0.0f, Color);
}

void Brick::ReduceLife()
{
	--Life;
	if (Life == 1)
		Color = glm::vec3(0.0f, 1.0f, 0.0f);
	else if (Life == 0)
		Destroyed = true;
}