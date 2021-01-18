#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <tuple>

#include "game_level.h"

const glm::vec2 PADDLE_SIZE(100.0f, 20.0f);

const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
const float BALL_RADIUS = 12.5f;


class Game
{
public:
	Game(unsigned int width, unsigned int height);
	~Game();

	void Init();
	void ProcessInput(float dt);
	void Update(float dt);
	void Render();

	void ResetGame();

	void SetKey(int key) { keys[key] = true; }
	void ResetKey(int key) { keys[key] = false; }

private:
	unsigned int width;
	unsigned int height;
	bool keys[1024];
	GameLevel level;
	Texture2D backgroundTexture;
};