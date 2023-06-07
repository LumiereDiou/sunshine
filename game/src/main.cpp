#include "rlImGui.h"
#include "Math.h"
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
    Color rightColor;
    Color leftColor;
};

void Update(Rigidbody& rb, float dt)
{
    rb.velocity = rb.velocity + rb.acceleration * dt;
    rb.position = rb.position + rb.velocity * dt + rb.acceleration * dt * dt * 0.5f;
    rb.direction = RotateTowards(rb.direction, Normalize(rb.velocity), rb.angularSpeed * dt);
}

Vector2 Seek(const Vector2& pos, const Rigidbody& rb, float maxSpeed)
{
    return Normalize(pos - rb.position) * maxSpeed - rb.velocity;
}

class Agent : public Rigidbody
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

    void ObstacleAvoidance(Agent& obstacle, Vector2 leftEnd, Vector2 rightEnd ,  float dt)
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
        rightColor = rightCollision ? RED : GREEN;
        leftColor = leftCollision ? RED : GREEN;
    }
};

int main(void)
{
    Vector2 result = Normalize(Vector2{ 30.0f, 70.0f });

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Asher - Lab3");
    rlImGuiSetup(true);
    SetTargetFPS(60);

    float seekerProbeLength = 100.0f;
    float seekerRadius = 25.0f;
    Agent seeker;
    seeker.position = { SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f };
    seeker.direction = { 0.0f, 1.0f };
    seeker.angularSpeed = 100.0f * DEG2RAD;

    Agent obstacle;
    obstacle.position = { SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT * 0.25f };
    obstacle.radius = 50.0f;

    bool useGUI = false;
    while (!WindowShouldClose())
    {
        const float dt = GetFrameTime();

        Vector2 right = Rotate(seeker.direction, 30.0f * DEG2RAD);
        Vector2 left = Rotate(seeker.direction, -30.0f * DEG2RAD);
        Vector2 rightEnd = seeker.position + right * seekerProbeLength;
        Vector2 leftEnd = seeker.position + left * seekerProbeLength;

        Vector2 right2 = Rotate(seeker.direction, 210.0f * DEG2RAD);
        Vector2 left2 = Rotate(seeker.direction, -210.0f * DEG2RAD);
        Vector2 rightEnd2 = seeker.position + right2 * seekerProbeLength;
        Vector2 leftEnd2 = seeker.position + left2 * seekerProbeLength;

        seeker.acceleration = Seek(GetMousePosition(), seeker, 100.0f);
        Update(seeker, dt);
        
        seeker.ObstacleAvoidance(obstacle, leftEnd, rightEnd, dt);
        seeker.ObstacleAvoidance(obstacle, leftEnd2, rightEnd2, dt);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawCircleV(seeker.position, seekerRadius, BLUE);
        DrawCircleV(obstacle.position, obstacle.radius, GRAY);
        DrawLineV(seeker.position, rightEnd, seeker.rightColor);
        DrawLineV(seeker.position, leftEnd, seeker.leftColor);

        DrawLineV(seeker.position, rightEnd2, seeker.rightColor);
        DrawLineV(seeker.position, leftEnd2, seeker.leftColor);

        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();
    return 0;
}