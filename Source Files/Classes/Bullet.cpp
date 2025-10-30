#include "Bullet.h"
#include "Frenemy.h"
#include <raylib.h>
#include <raymath.h>

//Default Constructor
Bullet::Bullet()
{
	position = { 0, 0 };
	velocity = { 0, 0 };
	color = BLANK;
	isAlive = false;
	lifeTimer = 0.0f;
	owner = nullptr; //pointer that doesnt point yet
}

//Convenient constructor
Bullet::Bullet(Vector2 startPos, Vector2 direction, float speed, Color color, Person* owner)
{
	this->owner = owner;
	this->position = startPos;
	this->velocity = Vector2Scale(direction, speed);
	this->color = color;
	this->isAlive = true;
	this->lifeTimer = 2.0f; // seconds of lifetime
}



void Bullet::Update(std::vector<Frenemy>& frenemies)
{
	if (!isAlive) return;

	// Move bullet
	position = Vector2Add(position, Vector2Scale(velocity, GetFrameTime()));

	// lifetime
	lifeTimer -= GetFrameTime();
	if (lifeTimer <= 0.0f) {
		isAlive = false;
		return;
	}

	// Check for collisions with frenemies
	for (Frenemy& frenemy : frenemies)
	{
		// bullets kill only alive frenemies that dont own the bullet
		if (frenemy.IsAlive() && &frenemy != owner)
		{
			if (CheckCollisionCircles(this->position, 5.0f, frenemy.GetCenter(), 20.0f))
			{
				frenemy.Kill();
			
				this->isAlive = false; // The bullet is now used up
				return; // Exit the function immediately since the bullet is dead
			}
		}
	}
}

void Bullet::Draw() const
{
	if (isAlive)
	{
		DrawCircleV(position, 5.f, color);
	}
}