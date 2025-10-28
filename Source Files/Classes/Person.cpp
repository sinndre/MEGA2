#include "Person.h"


//Constructor
Person::Person(Vector2 startPos, float speed, Color color) {
    this->position = startPos;
    this->speed = speed;
    this->color = color;
    this->size = { 40.0f, 40.0f };
}

void Person::Update() {
    // Empty 
}

//Draw persons body
void Person::Draw()const {
    DrawRectangleV(position, size, color);
}

Vector2 Person::GetCenter() const {
    // Calculate the center by adding half the size to the top-left position
    return { position.x + size.x / 2.0f, position.y + size.y / 2.0f };
}

//Getter
Vector2 Person::GetPosition() const {
    return position;
}