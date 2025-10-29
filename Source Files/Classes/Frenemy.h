#pragma once 
#include "Person.h"
#include <raymath.h> 

#include <vector>

class Bullet;

class Frenemy : public Person
{
public:
    // Constructor
    Frenemy(Vector2 startPos, float speed, Color color);

   
    
    float respawnTimer = 10.f;
    void Respawn();

    void Update(Vector2 playerPosition, std::vector<Bullet>& bulletContainer, bool isPlayerShouting);

    float GetDistanceToPlayer(const Vector2& playerPos) const;

    void Draw() const override;

    enum Affiliation
    {
        mega,
        antimega,
        neutral,
    
    };

    int state =Affiliation::neutral;
    bool isCloseToPlayer = false;
    void ReactToShout();

private:
    
    float shootCooldown = 0.0f;
    bool isRetreating = false;
};