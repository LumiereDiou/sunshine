#include <iostream>
#include "rlImGui.h"
#include "raylib.h"
#include "Tilemap.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

void RegenerateLevel(Tilemap& level, float chanceOfWall = 20)
{
    for (int x = 0; x < level.GetWidth(); x++)
    {
        for (int y = 0; y < level.GetHeight(); y++)
        {
            if ((rand() % 100 + 1) < chanceOfWall)
                level.SetTile(x, y, Tile::Wall);
            else
                level.SetTile(x, y, Tile::Floor);
        }
    }
}

//Tilemap tileMap(32, 18);
Vector2 character{ 0, 0 };
Vector2 updatedPosition;

Tilemap tileMap;

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Asher - Lab_5");

    rlImGuiSetup(true);

    bool useGUI = false;

    SetTargetFPS(60); //set framrate to 10 to get desired target movement

    //tileMap.GenerateLevel();

    RegenerateLevel(tileMap);

    while (!WindowShouldClose())
    {

       //if (IsKeyDown(KEY_W) && tileMap.IsTraversableAt(character.x, character.y - 1))
       //    updatedPosition.y--;
       //if (IsKeyDown(KEY_S) && tileMap.IsTraversableAt(character.x, character.y + 1))
       //    updatedPosition.y++;
       //if (IsKeyDown(KEY_A) && tileMap.IsTraversableAt(character.x - 1, character.y))
       //    updatedPosition.x--;
       //if (IsKeyDown(KEY_D) && tileMap.IsTraversableAt(character.x + 1, character.y))
       //    updatedPosition.x++;
       //
       //if (updatedPosition.x != character.x || updatedPosition.y != character.y)
       //{
       //    character = updatedPosition;
       //}
       //
       //tileMap.CreateAdjacentTiles();

        BeginDrawing();

        ClearBackground(RAYWHITE);
        
        tileMap.DrawTiles();

        tileMap.DrawBorders();

        tileMap.DrawAdjacencies();

       // tileMap.DrawTiles();
       //
       // tileMap.DrawAdjacentLines();

        if (IsKeyPressed(KEY_GRAVE)) useGUI = !useGUI;
        if (useGUI)
        {

            rlImGuiBegin();

            if (ImGui::Button("Generate map"))
            {
               // tileMap.GenerateLevel();
                RegenerateLevel(tileMap);
            }

            rlImGuiEnd();
        }

        

        // Draw character sprite
       //Rectangle characterRect{ character.x * tileMap.GetWidth(), character.y * tileMap.GetWidth(), tileMap.GetWidth(), tileMap.GetWidth()};
       //
       //DrawRectangleRec(characterRect, BLACK);
        //
        //DrawText("Press ~ to open/close GUI", 10, 30, 20, BLACK);

        EndDrawing();
    }

    rlImGuiShutdown();

    CloseWindow();

    return 0;
}
