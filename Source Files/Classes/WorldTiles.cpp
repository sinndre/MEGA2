#include "WorldTiles.h"
#include <raylib.h> // We need this for functions like GetMonitorWidth, SetRandomSeed, etc.

// Constructor can be empty if there's nothing to initialize
GrassGenerator::GrassGenerator()
{
}

// All the drawing logic is now inside this method
void GrassGenerator::Draw(const Camera2D& camera)
{
    // Detect where the camera is looking to generate grass in the correct place.
    int cameraTileX = (camera.target.x / TILE_SIZE);
    int cameraTileY = (camera.target.y / TILE_SIZE);

    // Calculate how many tiles we need to draw to fill the screen
    int viewTilesX = (GetMonitorWidth(0) / TILE_SIZE / camera.zoom / 2) + 2;
    int viewTilesY = (GetMonitorHeight(0) / TILE_SIZE / camera.zoom / 2) + 2;

    // Loop through visible tiles around the camera
    for (int x = cameraTileX - viewTilesX; x <= cameraTileX + viewTilesX; x++) {
        for (int y = cameraTileY - viewTilesY; y <= cameraTileY + viewTilesY; y++) {

            // Random seed plus prime numbers. The grass will stay the same after it is generated.
            SetRandomSeed(x * 13 + y * 53);

            for (int i = 0; i < GRASS_PER_TILE; i++) {
                // Generate one blade of grass within this tile
                float bladeX = x * TILE_SIZE + GetRandomValue(0, TILE_SIZE);
                float bladeY = y * TILE_SIZE + GetRandomValue(0, TILE_SIZE);

                float height = GetRandomValue(8, 15);
                float bend = GetRandomValue(-5, 5);
                Color color = { (unsigned char)GetRandomValue(40, 60),
                                (unsigned char)GetRandomValue(160, 190),
                                (unsigned char)GetRandomValue(50, 70), 255 };

                // Define the blade's shape
                Vector2 p1 = { bladeX, bladeY };
                Vector2 p2 = { bladeX + bend, bladeY - height * 0.6f };
                Vector2 p3 = { p2.x - bend * 0.8f, p2.y - height * 0.7f };

                DrawLineEx(p1, p2, 1.5f, color);
                DrawLineEx(p3, p2, 1.5f, color);
            }
        }
    }
}