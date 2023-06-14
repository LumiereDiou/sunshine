#include <iostream>
#include "rlImGui.h"
#include "raylib.h"
#include "Tilemap.h"

Tilemap tilemap(32, 18);
Vector2 character{ 0, 0 };
Vector2 updatedPosition;

int main()
{
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Asher - Lab_4");

    rlImGuiSetup(true);

    bool useGUI = false;

    SetTargetFPS(10); //set framrate to 10 to get desired target movement

    tilemap.GenerateLevel();
    tilemap.CreateAdjacentTiles();

    while (!WindowShouldClose())
    {
        updatedPosition = character;

        if (IsKeyDown(KEY_W) && tilemap.IsTileTraversable(character.x, character.y - 1))
            updatedPosition.y--;
        if (IsKeyDown(KEY_S) && tilemap.IsTileTraversable(character.x, character.y + 1))
            updatedPosition.y++;
        if (IsKeyDown(KEY_A) && tilemap.IsTileTraversable(character.x - 1, character.y))
            updatedPosition.x--;
        if (IsKeyDown(KEY_D) && tilemap.IsTileTraversable(character.x + 1, character.y))
            updatedPosition.x++;

        if (updatedPosition.x != character.x || updatedPosition.y != character.y)
        {
            character = updatedPosition;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        tilemap.DrawTiles();

        tilemap.DrawAdjacentLines();

        if (IsKeyPressed(KEY_GRAVE)) useGUI = !useGUI;
        if (useGUI)
        {

            rlImGuiBegin();

            if (ImGui::Button("Generate map"))
            {
                tilemap.GenerateLevel();
            }

            rlImGuiEnd();
        }

        // Draw character sprite
        Rectangle characterRect{ character.x * TILE_SIZE, character.y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
        DrawRectangleRec(characterRect, BLACK);

        //DrawText("Press ~ to open/close GUI", 10, 30, 20, GRAY);

        EndDrawing();
    }

    rlImGuiShutdown();

    CloseWindow();

    return 0;
}
