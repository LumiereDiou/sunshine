#include "rlImGui.h"
#include "Math.h"
#include <vector>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

bool CheckCollisionLineCircle(Vector2 lineStart, Vector2 lineEnd, Vector2 circlePosition, float circleRadius)
{
    Vector2 nearest = NearestPoint(lineStart, lineEnd, circlePosition);
    return DistanceSqr(nearest, circlePosition) <= circleRadius * circleRadius;
}

class Rigidbody
{
public:
    Vector2 position{};
    Vector2 velocity{};
    Vector2 acceleration{};

    Vector2 direction{};
    float angularSpeed;
    float radius;
};

class Agent : public Rigidbody
{
public:
    Agent(float x, float y)
    {
        position.x = x;
        position.y = y;
        direction = { 0.0f, 1.0f };
        angularSpeed = 100.0f * DEG2RAD;
        this->radius = 20.0f;
    }

    Agent(float x, float y, float radius)
    {
        position.x = x;
        position.y = y;
        direction = { 0.0f, 1.0f };
        angularSpeed = 100.0f * DEG2RAD;
        this->radius = radius;
    }

    void ObstacleAvoidance(Agent& obstacle, Vector2 leftEnd, Vector2 rightEnd, float dt)
    {
        bool leftCollision = CheckCollisionLineCircle(position, leftEnd, obstacle.position, obstacle.radius);
        bool rightCollision = CheckCollisionLineCircle(position, rightEnd, obstacle.position, obstacle.radius);
        if (rightCollision)
        {
            Vector2 linearDirection = Normalize(velocity);
            float linearSpeed = Length(velocity);
            velocity = Rotate(linearDirection, -angularSpeed * dt) * linearSpeed;
        }
        else if (leftCollision)
        {
            Vector2 linearDirection = Normalize(velocity);
            float linearSpeed = Length(velocity);
            velocity = Rotate(linearDirection, angularSpeed * dt) * linearSpeed;
        }
    }


};

void Update(Agent& agent, float deltaTime)
{
    agent.velocity = agent.velocity + agent.acceleration * deltaTime;
    agent.position = agent.position + agent.velocity * deltaTime + agent.acceleration * deltaTime * deltaTime * 0.5f;
    agent.direction = RotateTowards(agent.direction, Normalize(agent.velocity), agent.angularSpeed * deltaTime);
}

Vector2 Seek(const Vector2& position, const Agent& agent, float maxSpeed)
{
    return Normalize(position - agent.position) * maxSpeed - agent.velocity;
}

int main(void)
{
    Vector2 result = Normalize(Vector2{ 30.0f, 70.0f });

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Asher - Lab3");
    rlImGuiSetup(true);
    SetTargetFPS(60);

    Agent* agent1 = new Agent(500, 100);
    Agent* agent2 = new Agent(400, 200);
    Agent* agent3 = new Agent(300, 300);
    Agent* agent4 = new Agent(200, 400);
    Agent* agent5 = new Agent(100, 500);

    std::vector<Agent*> agentList;
    std::vector<Agent*> obstacleList;

    agentList.push_back(agent1);
    agentList.push_back(agent2);
    agentList.push_back(agent3);
    agentList.push_back(agent4);
    agentList.push_back(agent5);

    float agentProbeLength = 100.0f;
    float obstacleRadius = 50.0f;

    Vector2 right;
    Vector2 left;
    Vector2 rightEnd;
    Vector2 leftEnd;

    Vector2 right2;
    Vector2 left2;
    Vector2 rightEnd2;
    Vector2 leftEnd2;

    while (!WindowShouldClose())
    {
        const float dt = GetFrameTime();

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Vector2 position = GetMousePosition();
            obstacleList.push_back(new Agent(position.x, position.y, obstacleRadius));
        }

        for (auto i : agentList)
        {
            //create whiskers for each agent
            right = Rotate(i->direction, 30.0f * DEG2RAD);
            left = Rotate(i->direction, -30.0f * DEG2RAD);
            rightEnd = i->position + right * agentProbeLength;
            leftEnd = i->position + left * agentProbeLength;
            
            //create whiskers that are 180 from the first
            right2 = Rotate(i->direction, 210.0f * DEG2RAD);
            left2 = Rotate(i->direction, -210.0f * DEG2RAD);
            rightEnd2 = i->position + right2 * agentProbeLength;
            leftEnd2 = i->position + left2 * agentProbeLength;

            //implement seek for all agents
            i->acceleration = Seek(GetMousePosition(), *i, 100.0f);
            
            Update(*i, dt);

            for (auto x : obstacleList)
            {
                //run obstacle avoidance for each agent against every obstacle
                i->ObstacleAvoidance(*x, leftEnd, rightEnd, dt);
                i->ObstacleAvoidance(*x, leftEnd2, rightEnd2, dt);
            }

            //draw each agent and it's whiskers
            DrawCircleV(i->position, i->radius, BLUE);
            DrawLineV(i->position, rightEnd, GREEN);
            DrawLineV(i->position, leftEnd, GREEN);
            DrawLineV(i->position, rightEnd2, RED);
            DrawLineV(i->position, leftEnd2, RED);
         
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        //draw obstacles
        for (auto i : obstacleList)
        {
            DrawCircleV(i->position, i->radius, GRAY);
        }

        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();
    return 0;
}