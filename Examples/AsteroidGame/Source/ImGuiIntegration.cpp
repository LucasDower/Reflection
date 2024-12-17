#include "ImGuiIntegration.hpp"

#include <functional>
#include "imgui.h"
#include "raylib.h"
#include <memory>

void ImGui::Reflection::ReflectedClass(RClass* Class)
{
    const std::function<void(RClass*, const std::string)> ShowProperties = [&](RClass* InClass, const std::string Name) -> void
        {
            ImGui::PushID(InClass);

            if (Name.empty())
            {
                ImGui::TextColored(ImVec4(1.0, 1.0, 0.0, 1.0), InClass->GetName().c_str());
                ImGui::Indent(32.0f);
            }
            else
            {
                ImGui::Text(Name.c_str());
                ImGui::SameLine();
                ImGui::TextColored(ImVec4(1.0, 1.0, 0.0, 1.0), InClass->GetName().c_str());
                ImGui::Indent(32.0f);
            }

            const RClassProperties& Props = InClass->GetProperties();

            for (const RProperty& Prop : Props)
            {
                if (Prop.Type == EDataType::UnsignedChar)
                {
                    ImGui::SliderInt(Prop.Name.c_str(), &InClass->GetPropertyValue<int>(Prop), 0, 255);
                    ImGui::SameLine();
                    ImGui::TextColored(ImVec4(1.0, 1.0, 0.0, 1.0), "Unsigned Char");
                }
                else if (Prop.Type == EDataType::Int)
                {
                    ImGui::InputInt(Prop.Name.c_str(), &InClass->GetPropertyValue<int>(Prop));
                    ImGui::SameLine();
                    ImGui::TextColored(ImVec4(1.0, 1.0, 0.0, 1.0), "Int");
                }
                else if (Prop.Type == EDataType::Boolean)
                {
                    ImGui::Checkbox(Prop.Name.c_str(), &InClass->GetPropertyValue<bool>(Prop));
                    ImGui::SameLine();
                    ImGui::TextColored(ImVec4(1.0, 1.0, 0.0, 1.0), "Bool");
                }
                else if (Prop.Type == EDataType::Float)
                {
                    ImGui::InputFloat(Prop.Name.c_str(), &InClass->GetPropertyValue<float>(Prop));
                    ImGui::SameLine();
                    ImGui::TextColored(ImVec4(1.0, 1.0, 0.0, 1.0), "Float");
                }
                else if (Prop.Type == EDataType::RObject)
                {
                    RClass& ClassProp = InClass->GetPropertyValue<RClass>(Prop);
                    ShowProperties(&ClassProp, Prop.Name);
                }
                else if (Prop.Type == EDataType::RObjectPointer)
                {
                    RClass*& ClassProp = InClass->GetPropertyValue<RClass*>(Prop);
                    if (ClassProp)
                    {
                        RClass& ClassPropRef = *ClassProp;
                        ShowProperties(&ClassPropRef, Prop.Name);
                    }
                    else
                    {
                        ImGui::Text(Prop.Name.c_str());
                        ImGui::SameLine();
                        ImGui::Text("= nullptr");
                    }
                }
                else if (Prop.Type == EDataType::RObjectVector)
                {
                    ImGui::Text(Prop.Name.c_str());
                    ImGui::Indent(32.0f);

                    std::vector<std::shared_ptr<RClass>>& Vec = InClass->GetPropertyValue<std::vector<std::shared_ptr<RClass>>>(Prop);
                    for (size_t Index = 0; Index < Vec.size(); ++Index)
                    {
                        ImGui::PushID(Index);

                        ImGui::Text(TextFormat("%d", Index));
                        ImGui::Indent(32.0f);
                        ShowProperties(Vec[Index].get(), "");
                        ImGui::Unindent(32.0f);

                        ImGui::PopID();
                    }
                    ImGui::Unindent(32.0f);
                }
                else
                {
                    ImGui::Text(Prop.Name.c_str());
                }
            }

            ImGui::Unindent(32.0f);

            ImGui::PopID();
        };

    ShowProperties(Class, "");
}