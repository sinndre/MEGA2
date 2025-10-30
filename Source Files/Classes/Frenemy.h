#pragma once
#include "Person.h"
#include <raymath.h>
#include <vector>

class Bullet;
class Frenemy; //Forward declare for vectorz

class Frenemy : public Person
{
public:
	// Constructor
	Frenemy(Vector2 startPos, float speed, Color color);

	float respawnTimer = 10.f;
	void Kill();
	void Respawn();
	bool deathProcessed = false; //Check if everything connected to the death of a frenemy has been done

	void Update(Vector2 playerPosition, std::vector<Frenemy>& allFrenemies, std::vector<Bullet>& bulletContainer, bool isPlayerShouting);

	float GetDistanceToPlayer(const Vector2& playerPos) const;

	void Draw() const override;

	enum Affiliation
	{
		mega,
		antimega,
		neutral,
	};

	int state = Affiliation::neutral;
	bool isCloseToPlayer = false;
	void ReactToShout();

	bool isShouting = false;
	int shoutingTick = 0;

private:

	float shootCooldown = 0.0f;
	bool isRetreating = false;
};