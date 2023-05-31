#include "rlImGui.h"
#include "Math.h"
#include <vector>


#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

class Rigidbody
{
public:
    Vector2 position{ SCREEN_WIDTH * 0.1f, SCREEN_HEIGHT * 0.9f };
    Vector2 velocity{};
    Vector2 acceleration{};

    const float maxSpeed = 500.0f, maxAcceleration = 500.0f;

    Rigidbody()
        : position{}
        , velocity{ 10.0f, 0.0f}
        , acceleration{50.0f,0.0f}
    {

    }

    Rigidbody(float x, float y)
    {
        position.x = x;
        position.y = y;
    }
    void update(Vector2 position, Vector2 velocity, Vector2 acceleration, float deltaTime)
    {
        this->position = position + velocity * deltaTime + acceleration * 0.5f * deltaTime * deltaTime;
        this->velocity = velocity + acceleration * deltaTime;
        float currentSpeed = Length(velocity);
        if (currentSpeed > maxSpeed)
        {
            this->velocity = velocity * (maxSpeed / currentSpeed);
        }
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

    Agent()
    {
        rigidBody = new Rigidbody();
    }
    Agent(float x, float y)
    {
        rigidBody = new Rigidbody();
        rigidBody->position.x = x;
        rigidBody->position.y = y;
    }
};

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sunshine");
    rlImGuiSetup(true);
    SetTargetFPS(60);
    
    Agent* target = new Agent();

    Agent* agent1 = new Agent(500,100);
    Agent* agent2 = new Agent(400,200);
    Agent* agent3 = new Agent(300,300);
    Agent* agent4 = new Agent(200,400);
    Agent* agent5 = new Agent(100,500);

    Rigidbody* obstacle1 = new Rigidbody(100, 676);
    Rigidbody* obstacle2 = new Rigidbody(356, 532);
    Rigidbody* obstacle3 = new Rigidbody(612, 388);
    Rigidbody* obstacle4 = new Rigidbody(868, 244);
    Rigidbody* obstacle5 = new Rigidbody(1124,100);

    std::vector<Agent*> agentList;
    std::vector<Rigidbody*> obstacleList;

    agentList.push_back(agent1);
    agentList.push_back(agent2);
    agentList.push_back(agent3);
    agentList.push_back(agent4);
    agentList.push_back(agent5);

    obstacleList.push_back(obstacle1);
    obstacleList.push_back(obstacle2);
    obstacleList.push_back(obstacle3);
    obstacleList.push_back(obstacle4);
    obstacleList.push_back(obstacle5);
    
    while (!WindowShouldClose())
    {
        const float deltaTime = GetFrameTime();
        
        for (auto i : agentList)
        {
            i->rigidBody->screenWrap(i->rigidBody->position);
            i->rigidBody->update(i->rigidBody->position, i->rigidBody->velocity, i->rigidBody->acceleration, deltaTime);
        }

        target->rigidBody->position = GetMousePosition();

        for (auto i : agentList)
        {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            {
                i->rigidBody->seek(i->rigidBody->position, i->rigidBody->velocity, target->rigidBody->position, i->rigidBody->maxSpeed, i->rigidBody->maxAcceleration);
            }
            else
            {
                for (auto object : obstacleList)
                {
                    i->rigidBody->flee(i->rigidBody->position, i->rigidBody->velocity, object->position, i->rigidBody->maxSpeed, i->rigidBody->maxAcceleration);
                }
            }
            
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (auto i : agentList)
        {
            DrawCircleV(i->rigidBody->position, 20.0f, RED);
        }
        for (auto i : obstacleList)
        {
            DrawCircleV(i->position, 20.0f, GREEN);
        }

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