#include "Frenemy.h"
#include "Bullet.h"
#include <raymath.h>
#include <stdlib.h>

// Constructor
Frenemy::Frenemy(Vector2 startPos, float speed, Color color)
	: Person(startPos, speed, color)
{
	// Frenemy stuff goes here
	isAlive = true;
}

void Frenemy::Respawn()
{
	//  spawn frenemy somewhere random in the big, big infinite MEGAworld
	float randX = GetRandomValue(-5000, 5000);
	float randY = GetRandomValue(-5000, 5000);
	position = { randX, randY };

	// Reset state
	state = Affiliation::neutral;
	color = RED;
	isAlive = true;
	
}

float Frenemy::GetDistanceToPlayer(const Vector2& playerPos) const
{
	float dx = playerPos.x - position.x;
	float dy = playerPos.y - position.y;
	float distanceToPlayer = sqrtf(dx * dx + dy * dy);
	return distanceToPlayer;
}

void Frenemy::Draw() const
{
	if (!IsAlive())
	{
		// Dedd frenemy is BLACK
		DrawRectangleV(position, size, BLACK);
		return;
	}

	//Alive frenemy acts as normal
	Person::Draw();
}

void Frenemy::ReactToShout()
{
	bool beingShoutedAt = true;

	if (isAlive && state != Affiliation::neutral)
	{
		return; // Do nothing if it has already picked a side
	}

	// Get a random number from 0 to 2
	int affiliationRoll = GetRandomValue(0, 2);

	switch (affiliationRoll)
	{
	case 0:
		state = Affiliation::mega;
		break;
	case 1:
		state = Affiliation::antimega;
		break;
	case 2:
		state = Affiliation::neutral;
		break;
	}
}

void Frenemy::Update(Vector2 playerPosition, std::vector<Bullet>& bulletContainer, bool isPlayerShouting)
{
	if (!isAlive)
	{

		return; 
	}

	// Define AI parameters
	const float retreatStartDistance = 650.0f; // Start retreating if closer than this
	const float retreatStopDistance = 700.0f;  // Stop retreating once farther than this
	const float followStopDistance = 300.0f;
	const float shootInterval = 1.2f;

	// Update cooldown timer
	if (shootCooldown > 0)
	{
		shootCooldown -= GetFrameTime();
	}

	float distance = Vector2Distance(position, playerPosition);
	isCloseToPlayer = (distance < 200);

	Vector2 directionToPlayer = Vector2Normalize(Vector2Subtract(playerPosition, position));

	//AI BEHAVIOR
	if (isAlive && state == Affiliation::antimega)
	{
		color = BLUE;

		if (distance < retreatStartDistance)
			isRetreating = true;

		else if (distance > retreatStopDistance)
		{
			isRetreating = false;
		}

		Vector2 directionToPlayer = Vector2Normalize(Vector2Subtract(playerPosition, position));

		// BEHAVIOR 1: If too close, run away
		if (isRetreating)
		{
			Vector2 directionAway = Vector2Scale(directionToPlayer, -1.0f);
			position = Vector2Add(position, Vector2Scale(directionAway, speed * GetFrameTime()));
		}
		// BEHAVIOR 2: If shooting distance, stop and shoot
		else
		{
			if (shootCooldown <= 0)
			{
				shootCooldown = shootInterval;

				const float bulletOffset = 30.0f;// BulletOffset because the bullet needs to start outside frenemy hitbox
				Vector2 bulletSpawnPos = Vector2Add(GetCenter(), Vector2Scale(directionToPlayer, bulletOffset));

				bulletContainer.push_back(Bullet(bulletSpawnPos, directionToPlayer, 800.f, BLUE, this));
			}
		}
	}
	else
	{
		if (isAlive && state == Affiliation::mega)
		{
			color = YELLOW;
			if (distance > followStopDistance)
			{
				position = Vector2Add(position, Vector2Scale(directionToPlayer, speed * GetFrameTime()));
			}
		}

		else
		{
			if (isAlive && state == Affiliation::neutral) color = RED;
			// BEHAVIOR 1: If too close and shouted at, run away
			if (isCloseToPlayer && isPlayerShouting)
			{
				if (distance < retreatStartDistance) isRetreating = true;
				else if (distance > retreatStopDistance) isRetreating = false;

				if (isRetreating)
				{
					Vector2 directionAway = Vector2Scale(directionToPlayer, -1.0f);
					position = Vector2Add(position, Vector2Scale(directionAway, speed * GetFrameTime()));
				}
			}
			else
			{
				// back to normal
				isRetreating = false;
				if (distance > followStopDistance)
				{
					position = Vector2Add(position, Vector2Scale(directionToPlayer, speed * GetFrameTime()));
				}
			}
		}
	}
	
}