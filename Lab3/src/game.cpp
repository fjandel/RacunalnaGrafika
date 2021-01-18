#include "game.h"
#include "ball.h"
#include "renderer.h"
#include "paddle.h"
#include "collisions.h"


Renderer* renderer;
Paddle* paddle;
Ball* ball;

Game::Game(unsigned int width, unsigned int height) 
	: keys(), width(width), height(height)
{

}

Game::~Game()
{
	delete renderer;
	delete paddle;
	delete ball;
}

void Game::Init()
{
	level = GameLevel("res/levels/one.txt", width, height / 2);
	backgroundTexture = Texture2D("res/textures/background.jpg", false);
	// load shaders
	Shader shader("res/shaders/sprite.vs", "res/shaders/sprite.frag");

	// configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->width),
		static_cast<float>(this->height), 0.0f, -1.0f, 1.0f);
	
	shader.Use().SetInteger("image", 0);
	shader.SetMatrix4("projection", projection);

	// set render-specific controls
	renderer = new Renderer(shader);

	glm::vec2 paddlePosition = glm::vec2(this->width / 2.0f - PADDLE_SIZE.x / 2.0f, this->height - PADDLE_SIZE.y);
	paddle = new Paddle(paddlePosition, PADDLE_SIZE);

	glm::vec2 ballPosition = paddlePosition + glm::vec2(PADDLE_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
	ball = new Ball(ballPosition, BALL_RADIUS, INITIAL_BALL_VELOCITY);
}

void Game::ProcessInput(float dt)
{
	float velocity = 600.0f * dt;

	if (this->keys[GLFW_KEY_A])
	{
		if (paddle->Position.x >= 0.0f)
		{
			paddle->Position.x -= velocity;
			if (ball->Stuck)
				ball->Position.x -= velocity;
		}
	}
	if (this->keys[GLFW_KEY_D])
	{
		if (paddle->Position.x <= this->width - paddle->Size.x)
		{
			paddle->Position.x += velocity;
			if (ball->Stuck)
				ball->Position.x += velocity;
		}
	}
	if (this->keys[GLFW_KEY_SPACE])
		ball->Stuck = false;
}

void Game::Update(float dt)
{
	ball->Move(dt, this->width);
	CalculateCollisions(*ball, *paddle, level.Bricks);

	if (ball->Position.y >= this->height || level.IsCompleted())
	{
		this->ResetGame();
	}
}

void Game::Render()
{
	renderer->Draw(backgroundTexture, glm::vec2(0.0f, 0.0f), glm::vec2(this->width, this->height));
	level.Draw(*renderer);
	paddle->Draw(*renderer);
	ball->Draw(*renderer);
}

void Game::ResetGame()
{
	level.Reset();

	paddle->Size = PADDLE_SIZE;
	paddle->Position = glm::vec2(this->width / 2.0f - paddle->Size.x, this->height - paddle->Size.y);
	ball->Reset(paddle->Position + glm::vec2(PADDLE_SIZE.x / 2.0f - BALL_RADIUS, -(BALL_RADIUS * 2.0f)), INITIAL_BALL_VELOCITY);
}