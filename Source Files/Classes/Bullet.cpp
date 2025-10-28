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



void Bullet::Update()
{
	if (!isAlive) return;

	// Move bullet
	position = Vector2Add(position, Vector2Scale(velocity, GetFrameTime()));

	lifeTimer -= GetFrameTime();
	if (lifeTimer <= 0.0f) {
		isAlive = false;
		return;
	}

	
}

void Bullet::Draw() const
{
	if (isAlive)
	{
		DrawCircleV(position, 5.f, color);
	}
}