#include <iostream>
#include "rlImGui.h"
#include "raylib.h"
#include "Tilemap.h"
#include "Pathfinder.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720


Tilemap tileMap(32, 18);
Tilemap& level = tileMap;
TileCoord character{ 0, 0 };
TileCoord updatedPosition;

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Asher - Lab_5");

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

        TileCoord mouseTilePos = level.GetTileAtScreenPos(GetMousePosition());

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
        Vector2 characterCenter{ character.x * TILE_SIZE + TILE_SIZE / 2 , character.y * TILE_SIZE + TILE_SIZE / 2 };
        
        //DrawRectangleRec(characterRect, BLACK);
        
        DrawCircleV(characterCenter, 15, BLACK);

        DrawText("Press ~ to open/close GUI", 10, 30, 20, BLACK);

        EndDrawing();
    }

    rlImGuiShutdown();

    CloseWindow();

    return 0;
}
