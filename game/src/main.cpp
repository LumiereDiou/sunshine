#include "rlImGui.h"
#include "Math.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

using namespace ImGui;

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sunshine");
    
    SetTargetFPS(60);
    
    rlImGuiSetup(true);

    Vector2 position = { 100, 100 }; //Pixel
    Vector2 velocity = { 10, 0 }; // Pixel / second
    Vector2 acceleration = { 50,0 }; // Pixel/s/s
    float maxSpeed = 1000;
    float maxAcceleration = 1000;

    while (!WindowShouldClose())
    {
        const float deltaTime = GetFrameTime();
        
        BeginDrawing();
        
        ClearBackground(RAYWHITE);

        rlImGuiBegin();

        SliderFloat2("position", &(position.x), 0, SCREEN_WIDTH);
        SliderFloat2("velocity", &(velocity.x), -maxSpeed, maxSpeed);
        SliderFloat2("acceleration", &(acceleration.x), -maxAcceleration, maxAcceleration);

        rlImGuiEnd();

        Vector2 displacement = velocity * deltaTime; // px/s * s = px
        position = position + displacement;
        Vector2 deltaVelocity = acceleration * deltaTime;// px/s/s * s = px/s
        position = position + deltaVelocity;

        DrawCircleV(position, 50, GREEN);

        DrawFPS(10, 10);

        EndDrawing();
    }

    rlImGuiShutdown();

    CloseWindow();
    return 0;
}