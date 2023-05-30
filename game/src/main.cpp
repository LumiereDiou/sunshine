#include "rlImGui.h"
#include "Math.h"
#include <vector>


#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

const float deltaTime = GetFrameTime();

using namespace ImGui;

class RigidBody
{
public:
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;

    RigidBody()
        : position {100.0f,100.0f}
        , velocity {10.0f,0.0f}
        , acceleration{0.0f,50.0f}
    {

    }
};

class Agent
{
public:
    RigidBody* rigidBody;
    float maxSpeed;
    float maxAcceleration;

    Agent()
        : maxSpeed (1000)
        , maxAcceleration (1000)
    {
        rigidBody = new RigidBody();
    }

    void update()
    {
        rigidBody->position = rigidBody->position + rigidBody->velocity * deltaTime + 0.5f * maxAcceleration * deltaTime * deltaTime;
        rigidBody->velocity = rigidBody->velocity + maxAcceleration * deltaTime;
    }
        


    Vector2 Seek(Vector2 agentPosition, Vector2 velocity, Vector2 targetPosition, float maxAcceleration)
    {
        Vector2 toTarget = targetPosition - agentPosition;
        toTarget = { toTarget.x / Length(toTarget), toTarget.y / Length(toTarget) };
        Vector2 desiredVelocity = toTarget * maxSpeed;
        Vector2 deltaVelocity = desiredVelocity - velocity;
        Vector2 acceleration = { deltaVelocity.x / Length(deltaVelocity), deltaVelocity.y / Length(deltaVelocity) };
        acceleration = acceleration * maxAcceleration;
        velocity = velocity + acceleration * deltaTime;
        return acceleration;
    }

    Vector2 Flee(Vector2 agentPosition, Vector2 velocity, Vector2 targetPosition, float maxAcceleration)
    {

        Vector2 toTarget = targetPosition + agentPosition;
        toTarget = { toTarget.x / Length(toTarget), toTarget.y / Length(toTarget) };
        Vector2 desiredVelocity = toTarget * maxSpeed;
        Vector2 deltaVelocity = desiredVelocity - velocity;
        Vector2 acceleration = { deltaVelocity.x / Length(deltaVelocity), deltaVelocity.y / Length(deltaVelocity) };
        acceleration = acceleration * maxAcceleration;
        velocity = velocity + acceleration * deltaTime;
        return acceleration;
    }
};



int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sunshine");
    
    SetTargetFPS(60);
    
    rlImGuiSetup(true);

    Agent* seeker = new Agent();
    Agent* target = new Agent();

    while (!WindowShouldClose())
    {

        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) target->rigidBody->position = GetMousePosition();
        
        seeker->update();

        Vector2 seek = seeker->Seek(seeker->rigidBody->position, seeker->rigidBody->velocity,target->rigidBody->position,50);

        BeginDrawing();
        
        ClearBackground(RAYWHITE);

        DrawCircleV(seek, 50.0, BLUE);
        DrawCircleV(target->rigidBody->position, 50.0, BLACK);

        EndDrawing();
    }

    rlImGuiShutdown();

    CloseWindow();
    return 0;
}