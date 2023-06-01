#include "rlImGui.h"
#include "Math.h"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720


int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sunshine");
    rlImGuiSetup(true);
    SetTargetFPS(60);


    Vector2 position{ SCREEN_WIDTH * 0.1f, SCREEN_HEIGHT * 0.9f };
    Vector2 velocity{};
    Vector2 acceleration{};

    Vector2 target{ SCREEN_WIDTH * 0.9f, SCREEN_HEIGHT * 0.1f };
    float seekerSpeed = 500.0f;

    // "Fully brave" by default -> seeks cursor (1 = flees cursor)
    float bravery = 0.0f;

    while (!WindowShouldClose())
    {
        const float dt = GetFrameTime();
        position = position + velocity * dt + acceleration * 0.5f * dt * dt;
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

        acceleration = Lerp(seekAcceleration, fleeAcceleration, bravery);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        //DrawCircleV(A, rA, cA);
        //DrawCircleV(B, rB, cB);
        //DrawCircleV(Lerp(A, B, t), Lerp(rA, rB, t), Lerp(cA, cB, t));
        DrawLineV(position, target, BLACK);
        DrawCircleV(position, 20.0f, RED);
        DrawCircleV(target, 20.0f, BLUE);

        // Draw line 100 units above the seeker
        //Vector2 end = position + Vector2{0.0f, -100.0f};
        //DrawLineV(position, end, RED);
        //DrawText("100 pixels above", end.x, end.y, 20, RED);

        rlImGuiBegin();
        ImGui::SliderFloat("Interpolation", &bravery, 0.0f, 1.0f);
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