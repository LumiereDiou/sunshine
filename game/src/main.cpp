#include "rlImGui.h"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720


struct Circle
{
    int x = 0;
    int y = 0;
    int r = 0;
    Color color;
};

void drawCircle(Circle circle)
{
    DrawCircle(circle.x, circle.y, circle.r, circle.color);
}

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sunshine");
    SetTargetFPS(60);

    Circle circle = { 500,500, 50, DARKBLUE };

    while (!WindowShouldClose())
    {
        if (IsKeyDown(KEY_RIGHT)) circle.x += 2;
        if (IsKeyDown(KEY_LEFT)) circle.x -= 2;
        if (IsKeyDown(KEY_UP)) circle.y -= 2;
        if (IsKeyDown(KEY_DOWN)) circle.y += 2;

        BeginDrawing();
        ClearBackground(RAYWHITE);
        drawCircle(circle);

        //DrawLine(0, 0, 500, 500, RED);
        //DrawRectangle(535,535, 50, 50, BLUE);
        //DrawCircleV(circle, 50, MAROON);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}