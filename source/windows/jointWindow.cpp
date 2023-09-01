#include "JointWindow.h"
#include <SFML/Graphics.hpp>
#include "imgui.h"

JointWindow::JointWindow(Joint* joint)
{
    this->joint = joint;
}

void JointWindow::update(const bool simulating) const
{
    ImGui::SetNextWindowPos(ImVec2(900, 10), ImGuiCond_FirstUseEver);
    // Check if the window is open and draw its content
    if (ImGui::Begin("Joint Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::Text("Joint Position:");
        ImGui::Text("X: %.2f", joint->position.x);
        ImGui::Text("Y: %.2f", joint->position.y);


        if (!simulating)
        {
            ImGui::Separator();
            bool isFixed = joint->fixed;
            if (ImGui::Checkbox("Fixed", &isFixed))
            {
                joint->fixed = isFixed;
            }

            ImGui::Separator();

            ImGui::Text("Joint Color:");
            auto color = ImVec4(joint->color.r / 255.0f, joint->color.g / 255.0f, joint->color.b / 255.0f, 1.0f);
            if (ImGui::ColorEdit3("Color", reinterpret_cast<float*>(&color)))
            {
                joint->color = sf::Color(static_cast<sf::Uint8>(color.x * 255.0f),
                    static_cast<sf::Uint8>(color.y * 255.0f),
                    static_cast<sf::Uint8>(color.z * 255.0f));
            }

            ImGui::Separator();
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.0f, 0.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.6f, 0.0f, 0.0f, 1.0f));
            // Delete Button
            if (ImGui::Button("Delete"))
            {
                joint->deleted = true; // Set the deleted property
            }
            ImGui::PopStyleColor(3);

        }

        ImGui::End();
    }
}
