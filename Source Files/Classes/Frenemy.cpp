#include "Frenemy.h"
#include "Bullet.h"
#include <raymath.h>
#include <stdlib.h>
#include <cfloat> //Need this to check what is the MEGAlagrest number a float can hold

// Constructor
Frenemy::Frenemy(Vector2 startPos, float speed, Color color)
	: Person(startPos, speed, color)
{
	// Frenemy stuff goes here
	isAlive = true;
}

void Frenemy::Kill()
{
	isAlive = false;
	respawnTimer = 5.0f; 
	deathProcessed = false;
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
	deathProcessed = true;
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

void Frenemy::Update(Vector2 playerPosition, std::vector<Frenemy>& allFrenemies, std::vector<Bullet>& bulletContainer, bool isPlayerShouting)
{
	if (!isAlive)
	{
		respawnTimer -= GetFrameTime();
		if (respawnTimer <= 0)
		{
			
			Respawn();
		}
		return;
	}

	//Shouting timer
	if (isShouting)
	{
		shoutingTick++;
		if (shoutingTick > 60) // Shouts for 1 second
		{
			isShouting = false;
			shoutingTick = 0;
		}
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

	float distanceToPlayer = Vector2Distance(position, playerPosition);
	isCloseToPlayer = (distanceToPlayer < 200);

	Vector2 directionToPlayer = Vector2Normalize(Vector2Subtract(playerPosition, position));

	//AI BEHAVIOR
	if (isAlive && state == Affiliation::antimega)
	{
		color = BLUE;

		if (distanceToPlayer < retreatStartDistance)
			isRetreating = true;

		else if (distanceToPlayer > retreatStopDistance)
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
	else if (isAlive && state == Affiliation::mega)
	{
		color = YELLOW;

		Frenemy* antimegaTarget = nullptr;
		float minAntimegaDist = FLT_MAX; //Make a MEGAfloat

		for (auto& otherFrenemy : allFrenemies)
		{
			// Find closest, living greatnesshating frenemy
			if (&otherFrenemy != this && otherFrenemy.isAlive && otherFrenemy.state == Affiliation::antimega)
			{
				float dist = Vector2Distance(this->position, otherFrenemy.position);
				if (dist < minAntimegaDist)
				{
					minAntimegaDist = dist;
					antimegaTarget = &otherFrenemy;
				}
			}
		}
		const float shootingRange = 700.0f;

		//Shoot greatnesshating frenemy
		if (antimegaTarget != nullptr && minAntimegaDist <= shootingRange)
		{
			// Engage
			if (shootCooldown <= 0)
			{
				shootCooldown = shootInterval;
				Vector2 directionToTarget = Vector2Normalize(Vector2Subtract(antimegaTarget->position, this->position));

				const float bulletOffset = 30.0f;
				Vector2 bulletSpawnPos = Vector2Add(GetCenter(), Vector2Scale(directionToTarget, bulletOffset));

				//  YELLOW bullet
				bulletContainer.push_back(Bullet(bulletSpawnPos, directionToTarget, 800.f, YELLOW, this));
			}
		}
		else
		{
			Frenemy* targetFrenemy = nullptr;
			float minDistance = FLT_MAX;

			//Find neutral frenemies
			for (auto& otherFrenemy : allFrenemies)
			{
				if (&otherFrenemy != this && otherFrenemy.isAlive && otherFrenemy.state == Affiliation::neutral)
				{
					float dist = Vector2Distance(this->position, otherFrenemy.position);
					if (dist < minDistance)
					{
						minDistance = dist;
						targetFrenemy = &otherFrenemy;
					}
				}
			}

			// If a neutral target was found, chase and shout at it
			if (targetFrenemy != nullptr)
			{
				const float shoutDistance = 150.0f;
				Vector2 directionToTarget = Vector2Normalize(Vector2Subtract(targetFrenemy->position, this->position));

				if (minDistance > shoutDistance)
				{
					// Chase the target
					position = Vector2Add(position, Vector2Scale(directionToTarget, speed * GetFrameTime()));
				}
				else
				{
					// Shout
					if (!isShouting)
					{
						isShouting = true;
						shoutingTick = 0;
					}
				}
			}
			else
			{
				if (distanceToPlayer > followStopDistance)
				{
					position = Vector2Add(position, Vector2Scale(directionToPlayer, speed * GetFrameTime()));
				}
			}
		}
	}

	else // neutral
	{
		color = RED;

		//  follow player unless retreating
		if (distanceToPlayer > followStopDistance)
		{
			position = Vector2Add(position, Vector2Scale(directionToPlayer, speed * GetFrameTime()));
		}
	}
}