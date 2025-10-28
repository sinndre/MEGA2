#pragma once
#include<raylib.h>

class Person
{
public:
	//Constructor
	Person(Vector2 startPos, float speed, Color color);

	//Virtual stuff, of which the children will create their own versions
	virtual ~Person() = default;//Destructor
	virtual void Update();
	virtual void Draw() const; //I dont know why I need this const, added it after troubleshooting with AI.

	//Position Getter
	Vector2 GetPosition()const; //I think const means that 'this' fuction will not change the variable?
	Vector2 GetCenter() const; //Needed for proper collision

	bool IsAlive() const { return isAlive; };
	void Kill() { isAlive = false; };
	void Revive() { isAlive = true; };
	bool isAlive{ true };

	// 'protected' means this class and its children can access these directly.
protected:
	
	Vector2 position;
	float speed;
	Color color;
	Vector2 size;

};

