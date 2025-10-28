#include "Player.h"
#include <raymath.h>

//Constructor
Player::Player(Vector2 startPos, float speed, Color color)
    : Person(startPos, speed, color) { //Calls Person and gives it the arguments but not in a salty manner
//Nothing here, its all in Person
}

void Player::Update()
{
    if (!IsAlive()) return;

    //Speed at any given time (resets to speed every frame if not SPACE is down)
    float currentSpeed = speed;

    if (IsKeyDown(KEY_SPACE)) {
        currentSpeed = 15.f;
    }
    if (IsKeyDown(KEY_D) ) {
        position.x += currentSpeed;
    }
    if (IsKeyDown(KEY_A) ) {
        position.x -= currentSpeed;
    }
    if (IsKeyDown(KEY_W)) {
        position.y -= currentSpeed;
    }
    if (IsKeyDown(KEY_S) ) {
        position.y += currentSpeed;
    }
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && shouting==false ) 
    {
       shouting = true;

    }
    if (shouting == true)
    {
        shoutingtick++;
        if (shoutingtick > 120)
        {
            shouting = false;
            shoutingtick = 0;
        }
    }
}

void Player::Draw() const
{
    if (!IsAlive())
    {
        // Dedd player is BLACK
        DrawRectangleV(position, size, BLACK);
        return;
    }

    //Alive player acts as normal
    Person::Draw();
}

Vector2 Player::AimDirection(Camera2D& camera)
{
    Vector2 mouseScreenPos = GetMousePosition(); //mouse position on screen
    Vector2 mouseWorldPos = GetScreenToWorld2D(mouseScreenPos, camera); // convert to world position
    return Vector2Normalize(Vector2Subtract(mouseWorldPos, position));

}
