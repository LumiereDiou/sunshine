#include "rlImGui.h"
#include "Math.h"


#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

class Rigidbody
{
public:
    Vector2 position{ SCREEN_WIDTH * 0.1f, SCREEN_HEIGHT * 0.9f };
    Vector2 velocity{};
    Vector2 acceleration{};

    void update(Vector2 position, Vector2 velocity, Vector2 acceleration, float deltaTime)
    {
        this->position = position + velocity * deltaTime + acceleration * 0.5f * deltaTime * deltaTime;
        this->velocity = velocity + acceleration * deltaTime;
    }

    float screenWrap(Vector2 position)
    {
        if (position.y > SCREEN_HEIGHT) { return (this->position.y = 0.0f); }
        if (position.y < 0.0f) { return (this->position.y = SCREEN_HEIGHT); };
        if (position.x > SCREEN_WIDTH) { return (this->position.x = 0.0f); }
        if (position.x < 0.0f) { return (this->position.x = SCREEN_WIDTH); }
    }

    Vector2 seek(Vector2 agentPosition, Vector2 agentVelocity, Vector2 targetPosition, const float maxSpeed, const float maxAcceleration)
    {
        Vector2 seekDirection = Normalize(targetPosition - agentPosition);
        Vector2 seekVelocity = seekDirection * maxSpeed;
        Vector2 seekAcceleration = seekVelocity - agentVelocity;
        this->acceleration = Normalize(seekAcceleration) * maxAcceleration;

        return acceleration;
    }

    Vector2 flee(Vector2 agentPosition, Vector2 agentVelocity, Vector2 targetPosition, const float maxSpeed, const float maxAcceleration)
    {
        return seek(targetPosition, agentVelocity, agentPosition, maxSpeed, maxAcceleration);
    }
};

class Agent
{
public:
    Rigidbody* rigidBody;
    const float maxSpeed = 200.0f, maxAcceleration = 200.0f;

    Agent()
    {
        rigidBody = new Rigidbody();
    }
};

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sunshine");
    rlImGuiSetup(true);
    SetTargetFPS(60);

    Agent* agent = new Agent();
    Agent* target = new Agent();

    while (!WindowShouldClose())
    {
        const float deltaTime = GetFrameTime();
        
        agent->rigidBody->update(agent->rigidBody->position, agent->rigidBody->velocity, agent->rigidBody->acceleration, deltaTime);

        agent->rigidBody->screenWrap(agent->rigidBody->position);

        target->rigidBody->position = GetMousePosition();

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            agent->rigidBody->seek(agent->rigidBody->position, agent->rigidBody->velocity, target->rigidBody->position, agent->maxSpeed, agent->maxAcceleration);
        }
        else
        {
            agent->rigidBody->flee(agent->rigidBody->position, agent->rigidBody->velocity, target->rigidBody->position, agent->maxSpeed, agent->maxAcceleration);

        }
        
        

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawCircleV(agent->rigidBody->position, 20.0f, RED);

        DrawCircleV(target->rigidBody->position, 20.0f, BLUE);

        rlImGuiBegin();
        //ImGui::SliderFloat("Interpolation", &cowardice, 0.0f, 1.0f);
        //ImGui::SliderFloat("Interpolation", &t, 0.0f, 1.0f);
        //ImGui::SliderFloat("Seeker speed", &seekerSpeed, -100.0f, 100.0f);
        //ImGui::SliderFloat2("Target", &target.x, 0.0f, SCREEN_WIDTH);
        //ImGui::SliderFloat2("Position", &agent->rigidBody->position.x, 0.0f, SCREEN_WIDTH);
        //ImGui::SliderFloat2("Velocity", &agent->rigidBody->velocity.x, -100.0f, 100.0f);
        //ImGui::SliderFloat2("Acceleration", &acceleration.x, -100.0f, 100.0f);
        rlImGuiEnd();

        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();
    return 0;
}