#include "rlImGui.h"
#include "Math.h"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720


int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sunshine");
    rlImGuiSetup(true);
    SetTargetFPS(60);


    Vector2 position{ SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f };
    Vector2 velocity{};
    Vector2 acceleration{};

    Vector2 target{ SCREEN_WIDTH * 0.9f, SCREEN_HEIGHT * 0.1f };
    float seekerSpeed = 500.0f;

    // "Fully brave" by default -> seeks cursor (1 = flees cursor)
    float bravery = 0.0f;

    while (!WindowShouldClose())
    {
        const float dt = GetFrameTime();
        //position = position + velocity * dt + acceleration * 0.5f * dt * dt;
        velocity = velocity + acceleration * dt;
        if (position.y > SCREEN_HEIGHT) position.y = 0.0f;
        if (position.y < 0.0f) position.y = SCREEN_HEIGHT;
        if (position.x > SCREEN_WIDTH) position.x = 0.0f;
        if (position.x < 0.0f) position.x = SCREEN_WIDTH;

        target = GetMousePosition();

        // AB = B - A
        Vector2 seekDirection = Normalize(target - position);
        Vector2 seekVelocity = seekDirection * seekerSpeed;
        Vector2 seekAcceleration = seekVelocity - velocity;

        Vector2 fleeDirection = Normalize(position - target);
        Vector2 fleeVelocity = fleeDirection * seekerSpeed;
        Vector2 fleeAcceleration = fleeVelocity - velocity;
        
        
        Vector2 angle = Normalize(seekDirection);
        float angleFromVector = atan2f(angle.y, angle.x);
        angleFromVector = fmodf(angleFromVector + 2 * PI, 2 * PI);

        float angle30DegreesFromVector = angleFromVector - 15 * DEG2RAD;
        angle30DegreesFromVector = fmodf(angle30DegreesFromVector + 2 * PI, 2 * PI);
        Vector2 vectorAt30Degrees = Vector2{ cosf(angle30DegreesFromVector), sinf(angle30DegreesFromVector) };

        acceleration = Lerp(seekAcceleration, fleeAcceleration, bravery);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawCircleV(position, 20.0f, RED);
        DrawCircleV(target, 20.0f, BLUE);
        //DrawLineV(position, position + 100.0f, BLACK);
        DrawLineV(position, position + fleeDirection * 100.0f, BLACK);
       
        DrawLineV(position, position + angle * 100.0f, GREEN);
        DrawLineV(position, position + vectorAt30Degrees * 100.0f, GREEN);


        rlImGuiBegin();
        //ImGui::SliderFloat("Interpolation", &bravery, 0.0f, 1.0f);
        //ImGui::SliderFloat("Interpolation", &t, 0.0f, 1.0f);
        //ImGui::SliderFloat("Seeker speed", &seekerSpeed, -100.0f, 100.0f);
        //ImGui::SliderFloat2("Target", &target.x, 0.0f, SCREEN_WIDTH);
        //ImGui::SliderFloat2("Position", &position.x, 0.0f, SCREEN_WIDTH);
        //ImGui::SliderFloat2("Velocity", &velocity.x, -100.0f, 100.0f);
        //ImGui::SliderFloat2("Acceleration", &acceleration.x, -100.0f, 100.0f);
        rlImGuiEnd();

        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();
    return 0;
}