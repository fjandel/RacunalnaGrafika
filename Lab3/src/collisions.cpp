#include "collisions.h"
#include "game.h"

glm::vec2 Directions[] = {
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, -1.0f),
		glm::vec2(-1.0f, 0.0f)
};

void CalculateCollisions(Ball& ball, Paddle& paddle, std::vector<Brick>& bricks)
{
	for (Brick& brick : bricks)
		if (!brick.Destroyed)
			BallBrickCollision(ball, brick);

	BallPaddleCollision(ball, paddle);
}

void BallBrickCollision(Ball& ball, Brick& brick)
{
	glm::vec2 difference = calculateDifferenceVector(ball.GetCenter(), brick.Size, brick.Position);

	bool collision = glm::length(difference) < ball.Radius;
	if (collision)
	{
		if (!brick.IsSolid)
		{
			brick.ReduceLife();
		}

		Direction direction = calculateDirectionVector(difference);
		if (direction == Direction::LEFT || direction == Direction::RIGHT) // horizontal collision
		{
			ball.Velocity.x = -ball.Velocity.x;

			float penetration = ball.Radius - std::abs(difference.x);
			if (direction == Direction::LEFT)
				ball.Position.x += penetration; // move ball to right
			else
				ball.Position.x -= penetration; // move ball to left;
		}
		else // vertical collision
		{
			ball.Velocity.y = -ball.Velocity.y; // reverse vertical velocity
			// relocate
			float penetration = ball.Radius - std::abs(difference.y);
			if (direction == Direction::UP)
				ball.Position.y -= penetration; // move ball back up
			else
				ball.Position.y += penetration; // move ball back down
		}
	}
}

void BallPaddleCollision(Ball& ball, Paddle& paddle)
{
	glm::vec2 difference = calculateDifferenceVector(ball.GetCenter(), paddle.Size, paddle.Position);

	bool collision = glm::length(difference) < ball.Radius;
	if (collision && !ball.Stuck)
	{		
		float centerBoard = paddle.Position.x + paddle.Size.x / 2.0f;
		float distance = (ball.Position.x + ball.Radius) - centerBoard;
		float percentage = distance / (paddle.Size.x / 2.0f);

		glm::vec2 oldVelocity = ball.Velocity;
		ball.Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * 2.0f;
		ball.Velocity = glm::normalize(ball.Velocity) * glm::length(oldVelocity);
		ball.Velocity.y = -1.0f * abs(ball.Velocity.y);
	}
}

glm::vec2 calculateDifferenceVector(glm::vec2 center, glm::vec2 aabb_size, glm::vec2 aabb_position)
{
	glm::vec2 aabb_half_extens(aabb_size.x / 2.0f, aabb_size.y / 2.0f);
	glm::vec2 aabb_center(aabb_position.x + aabb_half_extens.x, aabb_position.y + aabb_half_extens.y);

	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extens, aabb_half_extens);
	glm::vec2 closest = aabb_center + clamped;

	difference = closest - center;
	
	return difference;
}

Direction calculateDirectionVector(glm::vec2 target)
{
	float max = 0.0f;
	unsigned int index_of_direction = -1;
	for (unsigned int i = 0; i < 4; i++)
	{
		float dot_product = glm::dot(glm::normalize(target), Directions[i]);
		if (dot_product > max)
		{
			max = dot_product;
			index_of_direction = i;
		}
	}

	return (Direction)index_of_direction;
}