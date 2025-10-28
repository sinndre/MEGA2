#pragma once
#include "raylib.h"
#include "Person.h"


class Player:public Person
{
public:
	//Constructor
	
	Player(Vector2 startPos, float speed, Color color);

	

	// override is the counterpart of the virtual in Person
	void Update()override; 
	Vector2 AimDirection(Camera2D& camera);
	bool shouting = false;

	int shoutingtick = 0;

	void Draw() const override;

};

