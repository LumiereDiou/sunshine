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

    Vector2 seekerPosition = { 100, 100 }; //Pixel
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

        SliderFloat2("position", &(seekerPosition.x), 0, SCREEN_WIDTH);
        SliderFloat2("velocity", &(velocity.x), -maxSpeed, maxSpeed);
        SliderFloat2("acceleration", &(acceleration.x), -maxAcceleration, maxAcceleration);
        rlImGuiEnd();

        seekerPosition = seekerPosition + velocity * deltaTime + 0.5f * acceleration * deltaTime * deltaTime;
        velocity = velocity + acceleration * deltaTime;// px/s/s * s = px/s
        
        
        Vector2 targetPosition = GetMousePosition();
        Vector2 lengthToTarget = targetPosition - seekerPosition;

  
        //direction = { direction.x / Length(direction),direction.y / Length(direction) }; //Normalized
       
        DrawCircleV(seekerPosition, 50, BLUE);
        DrawCircleV(targetPosition, 50, GRAY);
        DrawLineV(seekerPosition, seekerPosition + velocity, RED);
        DrawLineV(seekerPosition, seekerPosition + acceleration, GREEN);

        DrawFPS(10, 10);

        EndDrawing();
    }

    rlImGuiShutdown();

    CloseWindow();
    return 0;
}