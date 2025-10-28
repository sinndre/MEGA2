#pragma once
#include "raylib.h"
#include <vector>

#include "Person.h"

class Frenemy;

class Bullet
{
public:
	Vector2 position;
	Vector2 velocity;
	Color color;
	bool isAlive{ false };
	
	float lifeTimer{ 0.0f }; 
	Person* owner = nullptr;
	Bullet();

	//Constructor
	Bullet(Vector2 startPos, Vector2 direction, float speed, Color color, Person* owner);

	

	void Update(std::vector<Frenemy>& frenemies);

	void Draw() const; 
	
};

