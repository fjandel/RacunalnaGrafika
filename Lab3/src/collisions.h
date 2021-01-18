#pragma once

#include <glm/glm.hpp>
#include <tuple>
#include <vector>

#include "ball.h"
#include "paddle.h"
#include "brick.h"

enum class Direction {
	UP, RIGHT, DOWN, LEFT
};

void CalculateCollisions(Ball& ball, Paddle& paddle, std::vector<Brick>& bricks);
void BallPaddleCollision(Ball& ball, Paddle& paddle);
void BallBrickCollision(Ball& ball, Brick& brick);

glm::vec2 calculateDifferenceVector(glm::vec2 center, glm::vec2 aabb_size, glm::vec2 aabb_center);
Direction calculateDirectionVector(glm::vec2 target);