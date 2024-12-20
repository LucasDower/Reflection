#include <memory>
#include <functional>
#include <vector>

#include "raylib.h"
#include "raymath.h"

#include "imgui.h"
#include "rlImGui.h"

#include "Reflection.hpp"

class RVector2f : public RObject
{
    REFLECTION_CLASS_BEGIN(RVector2f, RObject)
        REFLECTION_PROPERTIES_BEGIN
            REFLECTION_PROPERTY(m_X)
            REFLECTION_PROPERTY(m_Y)
        REFLECTION_PROPERTIES_END
    REFLECTION_CLASS_END

public:
    float m_X = 0;
    float m_Y = 0;
};

class RPhysicsBody : public RObject
{
    REFLECTION_CLASS_BEGIN(RPhysicsBody, RObject)
        REFLECTION_PROPERTIES_BEGIN
            REFLECTION_PROPERTY(m_Position)
            REFLECTION_PROPERTY(m_Velocity)
        REFLECTION_PROPERTIES_END
    REFLECTION_CLASS_END

public:
    RVector2f m_Position;
    RVector2f m_Velocity;
};

class RPlayer : public RObject
{
    REFLECTION_CLASS_BEGIN(RPlayer, RObject)
        REFLECTION_PROPERTIES_BEGIN
            REFLECTION_PROPERTY(m_Health)
            REFLECTION_PROPERTY(m_Mana)
            REFLECTION_PROPERTY(m_Level)
            REFLECTION_PROPERTY(m_Experience)
            REFLECTION_PROPERTY(m_Position)
        REFLECTION_PROPERTIES_END
    REFLECTION_CLASS_END

public:
    int m_Health = 100;
    int m_Mana = 200;
    int m_Level = 0;
    float m_Experience = 0.0f;
    RVector2f m_Position;
};

class RPlayerWithLevel : public RObject
{
    REFLECTION_CLASS_BEGIN(RPlayerWithLevel, RPlayer)
        REFLECTION_PROPERTIES_BEGIN
            REFLECTION_PROPERTY(m_Level)
            REFLECTION_PROPERTY(m_Experience)
        REFLECTION_PROPERTIES_END
    REFLECTION_CLASS_END

public:
    int m_Level = 0;
    float m_Experience = 0.0f;
};

void ShowProperties(RObject& Object)
{
    const RClass& Class = Object.GetClass();

    for (size_t PropertyIndex = 0; PropertyIndex < Class.GetProperties().size(); ++PropertyIndex)
    {
        const RProperty& Property = Class.GetProperties()[PropertyIndex];

        const RClass& Class = Object.GetClass();

        ImGui::Text(Property.GetName().c_str());
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), RProperty::StaticGetDataTypeString(Property.GetType()).c_str());

        ImGui::Indent(16.0f);
        {
            ImGui::PushID(PropertyIndex);
            switch (Property.GetType())
            {
            case RProperty::Type::Int:
            {
                ImGui::InputInt("##", &Class.GetMutablePropertyValueChecked<int>(Object, Property));
                break;
            }
            case RProperty::Type::Float:
            {
                ImGui::InputFloat("##", &Class.GetMutablePropertyValueChecked<float>(Object, Property));
                break;
            }
            case RProperty::Type::RObject:
            {
                RObject& Subobject = Class.GetMutablePropertyValueChecked<RObject>(Object, Property);
                ShowProperties(Subobject);
                break;
            }
            default:
            {
                ImGui::TextDisabled("Unsupported");
            }
            }
            ImGui::PopID();
        }
        ImGui::Unindent(16.0f);
    }
}

void ShowHierarchy(RObject& Object)
{
    const RClass& Class = Object.GetClass();

    std::vector<std::string> Ancestors;
    const RClass* Parent = Class.GetParent();
    while (Parent)
    {
        Ancestors.push_back(Parent->GetName());
        Parent = Parent->GetParent();
    }

    for (auto It = Ancestors.rbegin(); It != Ancestors.rend(); ++It) {
        ImGui::Text(It->c_str());
        ImGui::Indent(16.0f);
    }
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), Class.GetName().c_str());
    ImGui::Unindent(16.0f * Ancestors.size());
}

int main(void)
{
    std::shared_ptr<RPhysicsBody> PhysicsBody = std::make_shared<RPhysicsBody>();

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(1920, 1080, "MyProject");
    SetTargetFPS(60);
    rlImGuiSetup(true);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(DARKGRAY);

        rlImGuiBegin();
        {
            ImGui::Begin("Details");
            {
                ShowHierarchy(*PhysicsBody);
                ImGui::Separator();
                ShowProperties(*PhysicsBody);
            }
            ImGui::End();
        }
        rlImGuiEnd();
            
        EndDrawing();
    }

    rlImGuiShutdown();

    return 0;
}