#include "Classes/Player.h"
#include "Classes/Frenemy.h"
#include "Classes/Bullet.h"
#include "Classes/WorldTiles.h"

#include <raylib.h>
#include <string>
#include <iostream>
#include <vector>
#include <raymath.h>
#include <algorithm>






int main()
{
	
	InitWindow(600, 600, "MEGA");
	SetWindowSize(GetMonitorWidth(0), GetMonitorHeight(0));
	ToggleFullscreen();

	SetTargetFPS(60);
    HideCursor();

	float halfScreenWidth = (float)(GetMonitorWidth(0) / 2);
	float halfScreenHeight = (float)(GetMonitorHeight(0) / 2);



	// Player Setup
	Player player({ 0, 0 }, 5.0f, YELLOW); //SPEED default is here

    // Bullet Setup
    std::vector<Bullet> bulletContainer;

    //Frenemy setup
    std::vector<Frenemy> FrenemyContainer;
    int amountOfFrenemies = 150;
    const float FrenemySpawndistance = 4000.0f;

    for (int i = 0; i < amountOfFrenemies; i++)
    {
        Vector2 FrenemySpawnPos; 

        //Loops spawn positions till it is far enough away
        do {
            // Random Frenemy spawn positions in a larger area
            float randX = GetRandomValue(-5000, 5000);
            float randY = GetRandomValue(-5000, 5000);
            FrenemySpawnPos = { randX, randY };

        } while (Vector2Distance(FrenemySpawnPos, player.GetPosition()) < FrenemySpawndistance);

        //Spawn Frenemy
        float randSpeed = GetRandomValue(150.0f, 250.0f);
        FrenemyContainer.push_back(Frenemy(FrenemySpawnPos, randSpeed, RED)); //push back adds new Frenemy at the end of the vector
    }

	//Camera Setup 
	Camera2D camera = { 0 };
	camera.target = player.GetPosition();  //  camera  targets the player
	camera.offset = { halfScreenWidth, halfScreenHeight }; //  centers the target on the screen
	camera.rotation = 0.0f;

	camera.zoom = 2.0f;
    float zoomSpeed = 0.1f;
    float minZoom = 0.5f;
    float maxZoom = 3.0f;

    GrassGenerator grassGenerator;

	//Game loop
	while (!WindowShouldClose())
	{
        Vector2 worldMousePos = GetScreenToWorld2D(GetMousePosition(), camera);
        float wheel = GetMouseWheelMove();


        if (wheel != 0)
        {
            // zoom level based on the scroll wheel and zoom speed
            camera.zoom += wheel * zoomSpeed;

            // Clamp the zoom 
            camera.zoom = Clamp(camera.zoom, minZoom, maxZoom);
        }
        
		//Updates
		player.Update();
        Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), camera);

        //Frenemy shout check
        for (auto& frenemy : FrenemyContainer)
        {
            
                // check for shout and distance to frenemy
                if (frenemy.isAlive && player.shouting && frenemy.isCloseToPlayer)
                {
                    frenemy.ReactToShout();
                }

                // Temporary panic
                bool shouldThisFrenemyPanic = (player.shouting && frenemy.isCloseToPlayer);
                // Update
                frenemy.Update(player.GetPosition(), bulletContainer, shouldThisFrenemyPanic);
            
        }

		camera.target = player.GetPosition();//Camera updates to player position

       


        // PLAYER Shoot bullets
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            Vector2 aimDirection = player.AimDirection(camera);

            const float bulletOffset = 25.0f; // BulletOffset because the bullet needs to start outside players hitbox
            Vector2 bulletSpawnPos = Vector2Add(player.GetCenter(), Vector2Scale(aimDirection, bulletOffset)); 

            bulletContainer.push_back(Bullet(bulletSpawnPos, aimDirection, 1000.f, YELLOW, &player));
            
        }

        for (Bullet& bulletObject : bulletContainer)
        {
            bulletObject.Update();
        }

        // Move bullets. In main because I need to check collision against everything
        for (Bullet& bullet : bulletContainer)
        {
            bullet.Update();
        }

        //Check collision for BULLETS IN GENERAL. In main because I need to check collision against everything
        for (Bullet& bullet : bulletContainer)
        {
            if (!bullet.isAlive) continue; // Skip dead bullets

            // Check bullets vs PLAYER
            if (player.IsAlive() && bullet.owner != &player)
            {
                if (CheckCollisionCircles(bullet.position, 5.0f, player.GetCenter(), 20.0f))
                {
                    player.Kill();
                    bullet.isAlive = false;
                    continue; // Bullet is used, move to the next bullet
                }
            }

            //Check bullets vs frenemies
            for (Frenemy& frenemy : FrenemyContainer)
            {
                if (frenemy.IsAlive() && bullet.owner != &frenemy)
                {
                    if (CheckCollisionCircles(bullet.position, 5.0f, frenemy.GetCenter(), 20.0f))
                    {
                        frenemy.Kill();
                        frenemy.respawnTimer = 5.0f;
                        bullet.isAlive = false;
                        break; // Bullet is used up, break from frenemy loop
                    }
                }
            }
        }

        //Bullet cleaner
        bulletContainer.erase(
            std::remove_if(bulletContainer.begin(), bulletContainer.end(), [](const Bullet& b) {
                return !b.isAlive;
                }),
            bulletContainer.end()
        );

        

		//Drawing
		BeginDrawing();

		ClearBackground({ 7, 44, 12, 255 });

		BeginMode2D(camera);//Drawing in "Mode2D" is affected by the camera

        


		
        // Call grass from WorldTiles class
        grassGenerator.Draw(camera);



        // Set seed back to something not really random for other stuff (after it is used by grass generator)
        SetRandomSeed(0);

		player.Draw(); // calls the Person::Draw() from Person

        if (player.shouting)
        {
            DrawText("MEGA!!!", static_cast<int>(player.GetPosition().x), static_cast<int>(player.GetPosition().y -30), 20, YELLOW);
        }
        
        //Loop through frenemies and draw them and their shout response
        for (const auto& frenemy : FrenemyContainer)
        {
            
                frenemy.Draw();
            
            if (frenemy.isAlive && frenemy.isCloseToPlayer && player.shouting)
            {
                if (frenemy.state == Frenemy::Affiliation::neutral)
                {
                    DrawText("uhh I dunno", frenemy.GetPosition().x, frenemy.GetPosition().y - 30, 20, RED);
                }
                if (frenemy.state == Frenemy::Affiliation::mega)
                {
                    DrawText("fuc yea! MEGA!!!", frenemy.GetPosition().x, frenemy.GetPosition().y - 30, 20, YELLOW);
                }
                if (frenemy.state == Frenemy::Affiliation::antimega)
                {
                    DrawText("OMG no", frenemy.GetPosition().x, frenemy.GetPosition().y - 30, 20, BLUE);
                }
            }
        }

        //Shoot bullets
        for (const auto& bullet : bulletContainer)
        {
            bullet.Draw();
        }

        float reticleRadius = 10.0f;
        float crosshairLength = 15.0f;
        Color reticleColor = YELLOW;
        // Draw mouse rectile
        DrawCircleLines(worldMousePos.x, worldMousePos.y, reticleRadius, reticleColor);
        DrawLine(worldMousePos.x - crosshairLength, worldMousePos.y, worldMousePos.x + crosshairLength, worldMousePos.y, reticleColor); // Horizontal line
        DrawLine(worldMousePos.x, worldMousePos.y - crosshairLength, worldMousePos.x, worldMousePos.y + crosshairLength, reticleColor); // Vertical line



        DrawText("Use WASD if you want, idc. Maybe press MOUSE buttons too or SPACE or maybe even SCROLL or whaeffa", -550, -100, 20, YELLOW);
        DrawText("QUITTERS press Esc, ADMITTING the QUITTING and that they are in fact QUITTERS", -550, -70, 20, YELLOW );


        EndMode2D();
		EndDrawing();
	}
	CloseWindow();

	return 0;
}