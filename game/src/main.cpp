#include <iostream>
#include "rlImGui.h"
#include "raylib.h"
#include "Tilemap.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

Tilemap tileMap(32, 18);
Vector2 character{ 0, 0 };
Vector2 updatedPosition;

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Asher - Lab_4");

    rlImGuiSetup(true);

    bool useGUI = false;

    SetTargetFPS(10); //set framrate to 10 to get desired target movement

    tileMap.GenerateLevel();

    while (!WindowShouldClose())
    {
        updatedPosition = character;

        if (IsKeyDown(KEY_W) && tileMap.IsTileTraversable(character.x, character.y - 1))
            updatedPosition.y--;
        if (IsKeyDown(KEY_S) && tileMap.IsTileTraversable(character.x, character.y + 1))
            updatedPosition.y++;
        if (IsKeyDown(KEY_A) && tileMap.IsTileTraversable(character.x - 1, character.y))
            updatedPosition.x--;
        if (IsKeyDown(KEY_D) && tileMap.IsTileTraversable(character.x + 1, character.y))
            updatedPosition.x++;

        if (updatedPosition.x != character.x || updatedPosition.y != character.y)
        {
            character = updatedPosition;
        }

        tileMap.CreateAdjacentTiles();

        BeginDrawing();

        ClearBackground(RAYWHITE);

        tileMap.DrawTiles();

        tileMap.DrawAdjacentLines();

        if (IsKeyPressed(KEY_GRAVE)) useGUI = !useGUI;
        if (useGUI)
        {

            rlImGuiBegin();

            if (ImGui::Button("Generate map"))
            {
                tileMap.GenerateLevel();
            }

            rlImGuiEnd();
        }

        // Draw character sprite
        Rectangle characterRect{ character.x * TILE_SIZE, character.y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
        
        DrawRectangleRec(characterRect, BLACK);

        DrawText("Press ~ to open/close GUI", 10, 30, 20, BLACK);

        EndDrawing();
    }

    rlImGuiShutdown();

    CloseWindow();

    return 0;
}
