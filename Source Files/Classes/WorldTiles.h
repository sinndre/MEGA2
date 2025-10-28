#ifndef GRASSGENERATOR_H
#define GRASSGENERATOR_H

#include <raylib.h>

class GrassGenerator
{
public:
    // Constructor
    GrassGenerator();

   
    void Draw(const Camera2D& camera);

private:
    // Constatnnts are not really constant but can be altered in the class
    static const int TILE_SIZE = 200;
    static const int GRASS_PER_TILE = 10;
};

#endif