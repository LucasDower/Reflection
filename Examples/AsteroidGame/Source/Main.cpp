#include "raylib.h"
#include "raymath.h"

#include <memory>
#include <functional>

#include "imgui.h"
#include "rlImGui.h"

#include "RClass.hpp"

#include "ImGuiIntegration.hpp"

class RVector2f : public RClass
{
    REFLECTED_CLASS(RVector2f, RClass)

public:
    float m_X;
    float m_Y;

    BEGIN_REFLECTED_PROPERTIES
        REFLECTED_PROPERTY(m_X)
        REFLECTED_PROPERTY(m_Y)
    END_REFLECTED_PROPERTIES
};

class RAsteroid : public RClass
{
    REFLECTED_CLASS(RAsteroid, RClass)

public:
    RVector2f Position;
    RVector2f Velocity;
    float Size;

    BEGIN_REFLECTED_PROPERTIES
        REFLECTED_PROPERTY(Position)
        REFLECTED_PROPERTY(Velocity)
        REFLECTED_PROPERTY(Size)
    END_REFLECTED_PROPERTIES
};

class RWorld : public RClass
{
    REFLECTED_CLASS(RWorld, RClass)

public:
    std::vector<std::shared_ptr<RAsteroid>> Asteroids;

    BEGIN_REFLECTED_PROPERTIES
        REFLECTED_PROPERTY(Asteroids)
    END_REFLECTED_PROPERTIES
};

int main(void)
{

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(1920, 1080, "MyProject");
    SetTargetFPS(60);
    rlImGuiSetup(true);

    std::shared_ptr<RWorld> World = std::make_shared<RWorld>();

    for (size_t Index = 0; Index < 10; ++Index)
    {
        std::shared_ptr<RAsteroid> Asteroid = std::make_shared<RAsteroid>();
        Asteroid->Position.m_X = GetRandomValue(0, GetScreenWidth());
        Asteroid->Position.m_Y = GetRandomValue(0, GetScreenHeight());
        Asteroid->Velocity.m_X = GetRandomValue(-100, 100);
        Asteroid->Velocity.m_Y = GetRandomValue(-100, 100);
        Asteroid->Size = GetRandomValue(8, 32);

        World->Asteroids.push_back(Asteroid);
    };

    size_t SelectedAsteroidIndex = 0;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(DARKGRAY);

        const float DeltaTime = GetFrameTime();

        for (size_t Index = 0; Index < World->Asteroids.size(); ++Index)
        {
            std::shared_ptr<RAsteroid> Asteroid = World->Asteroids[Index];
         
            Asteroid->Position.m_X += Asteroid->Velocity.m_X * DeltaTime;
            Asteroid->Position.m_Y += Asteroid->Velocity.m_Y * DeltaTime;

            Asteroid->Position.m_X = std::fmod(Asteroid->Position.m_X, (float)GetScreenWidth());
            Asteroid->Position.m_Y = std::fmod(Asteroid->Position.m_Y, (float)GetScreenHeight());
        }

        const Vector2 MousePos = GetMousePosition();

        for (size_t Index = 0; Index < World->Asteroids.size(); ++Index)
        {
            std::shared_ptr<RAsteroid> Asteroid = World->Asteroids[Index];
            DrawCircle(Asteroid->Position.m_X, Asteroid->Position.m_Y, Asteroid->Size, RED);

            Rectangle AsteroidBounds = { Asteroid->Position.m_X - Asteroid->Size, Asteroid->Position.m_Y - Asteroid->Size, Asteroid->Size * 2, Asteroid->Size * 2 };

            if (MousePos.x >= AsteroidBounds.x && MousePos.x <= (AsteroidBounds.x + AsteroidBounds.width) && MousePos.y >= AsteroidBounds.y && MousePos.y <= (AsteroidBounds.y + AsteroidBounds.height))
            {
                DrawRectangleLinesEx(AsteroidBounds, 1.0f, WHITE);

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    SelectedAsteroidIndex = Index;
                }
            }
        }

        if (SelectedAsteroidIndex != -1)
        {
            std::shared_ptr<RAsteroid> Asteroid = World->Asteroids[SelectedAsteroidIndex];
            DrawRectangleLines(Asteroid->Position.m_X - Asteroid->Size, Asteroid->Position.m_Y - Asteroid->Size, Asteroid->Size*2, Asteroid->Size*2, YELLOW);
        }

        rlImGuiBegin();
        {
            ImGui::Begin("Object");
            {
                ImGui::Reflection::ReflectedClass(World.get());
            }
            ImGui::End();

            ImGui::Begin("Details");
            {
                ImGui::Reflection::ReflectedClass(World->Asteroids[SelectedAsteroidIndex].get());
            }
            ImGui::End();
        }
        rlImGuiEnd();
            
        EndDrawing();
    }

    rlImGuiShutdown();

    return 0;
}