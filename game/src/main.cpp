#include <iostream>
#include "rlImGui.h"
#include "raylib.h"
#include "Tilemap.h"
#include "Pathfinder.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720


Tilemap tileMap(32, 18);
TileCoord character{ 0, 0 };
TileCoord updatedPosition;
Pathfinder pathfinder;

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

        TileCoord mouseTilePos = tileMap.GetTileAtScreenPos(GetMousePosition());

        if (tileMap.ContainsTile(mouseTilePos))
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
            {
                pathfinder = Pathfinder(&tileMap, character, TileCoord(mouseTilePos));
                pathfinder.map->DrawAdjacentLines();
            }
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                pathfinder = Pathfinder(&tileMap, character, TileCoord(mouseTilePos));
                pathfinder.SolvePath();
            }
        }

        if (pathfinder.map != nullptr)
        {
            if (IsKeyPressed(KEY_SPACE))
            {
                pathfinder.ProcessNextIterationFunctional();
            }

            //if (drawPathInfo) pathfinder.DrawCurrentState();
        }

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
        
        DrawCircleV(characterCenter, 15, BLACK);

        DrawText("Press ~ to open/close GUI", 10, 30, 20, BLACK);

        EndDrawing();
    }

    rlImGuiShutdown();

    CloseWindow();

    return 0;
}
