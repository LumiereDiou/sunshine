#include "rlImGui.h"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720


//struct Circle
//{
//    int x = 0;
//    int y = 0;
//    int r = 0;
//    Color color;
//};

//void drawCircle(Circle circle)
//{
//    DrawCircle(circle.x, circle.y, circle.r, circle.color);
//}



int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sunshine");
    SetTargetFPS(60);

    //InitAudioDevice();

    //Circle circle = { 500,500, 50, DARKBLUE };
    //Texture2D texture = LoadTexture("../Assets/image.png");
    //Sound sound = LoadSound("../Assets/audio.mp3");

    Vector2 startPos = {100.0f, 100.0f};
    Vector2 endPos = { 200.0f, 200.0f };
    Vector2 controlPos = { 100.0f, 200.0f };
    Vector2 controlPos2 = { 200.0f, 300.0f };
    Vector2 center1 = { 200.0f, 200.0f };
    Vector2 center2 = { SCREEN_WIDTH/2, SCREEN_HEIGHT/2 };
    float radius1 = 100.00;
    float radius2 = 100.00;
    float thick = 20.00;
    Color color1;
    Color color2;


    while (!WindowShouldClose())
    {
        //if (IsKeyDown(KEY_RIGHT)) circle.x += 2;
        //if (IsKeyDown(KEY_LEFT)) circle.x -= 2;
        //if (IsKeyDown(KEY_UP)) circle.y -= 2;
        //if (IsKeyDown(KEY_DOWN)) circle.y += 2;
        //if (IsKeyDown(KEY_SPACE)) PlaySound(sound);
        center1 = GetMousePosition();
        if (CheckCollisionCircles(center1, radius1, center2, radius2))
        {
            color1 = PURPLE;
            color2 = YELLOW;
        }
        else
        {
            color1 = RED;
            color2 = BLUE;
        }
        BeginDrawing();
        ClearBackground(RAYWHITE);
        //DrawTexture(texture, SCREEN_WIDTH/2 - texture.width/2, SCREEN_HEIGHT/2 - texture.height/2 , WHITE);
        //DrawText("Press SPACE to PLAY music!", 500, 500, 20, LIGHTGRAY);
        //drawCircle(circle);
        //DrawLine(0, 0, 500, 500, BLUE);
        //DrawRectangle(535,535, 50, 50, BLUE);
        
        ///////////ASSIGNMENT QUESTION 1 ANSWERS///////////////////////
        //DrawLineV(startPos,endPos,RED); // draws a vector line.. can be used to change the position of the line using any input method
        //DrawLineEx(startPos, endPos,20.0f , BLUE); //draws a line with thickness
        //DrawLineBezier(startPos,endPos,thick,color); //draws a curved line with thickness
        //DrawLineBezierQuad(startPos,endPos,controlPos,thick,color); //draws a curved line with control on where the curve begins
        //DrawLineBezierCubic(startPos,endPos,controlPos, controlPos2,thick,color);//draws a curved line with control on where the curve begins and ends
        //DrawCircleSector(center,200.0f,400.0f,500.0f,1,color);//draws part of a circle
        //DrawCircleSectorLines(center, 200.0f, 400.0f, 500.0f, 1, color);//draws part of a circle without filling it with color
        //DrawCircleGradient(500,500, 100.0,RED,BLUE);//draws a circle and fills it with two colors
        //DrawCircleV(center,100.00, color);//draws a color filled circle that can be moved using any input method
        //DrawCircleLines(500,500,100.0,color);//draws a circle without filling it with color

        ///////////ASSIGNMENT QUESTION 2 ANSWERS///////////////////////
        DrawCircleV(center1, radius1,color1);
        DrawCircleV(center2, radius2,color2);
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}