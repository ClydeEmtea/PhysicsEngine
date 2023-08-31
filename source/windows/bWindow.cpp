#include "BWindow.h"
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Window.hpp>
#include <iostream>
BWindow::BWindow() : selectedButton(0), isBeingDragged(false), isFocused(false) {
    windowPos = ImVec2(201, ImGui::GetIO().DisplaySize.y - 150);
}

void BWindow::update(bool& delAll, int& lastSelected, bool& stopSim, bool& startSim) {
    int buttonWidth = static_cast<int>(ImGui::GetIO().DisplaySize.x * 2 / 3) / 6 - 7;

    ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x * 2 / 3 + 2, 130));
    ImGui::SetNextWindowPos(windowPos, ImGuiCond_FirstUseEver);

    ImGui::Begin("Tool Selector [B]", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings);
    isFocused = ImGui::GetIO().WantCaptureMouse;

    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
        isBeingDragged = true;
        ImGui::SetDragDropPayload("WINDOW_DND", nullptr, 0);
        ImGui::EndDragDropSource();
    }

    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("WINDOW_DND")) {
            isBeingDragged = false;
            windowPos = ImVec2(ImGui::GetMousePos().x, windowPos.y);
        }
        ImGui::EndDragDropTarget();
    }

    const char* selectableLabels[] = { "Mouse [Q]", "Stick [W]", "Rope [E]", "Bulldozer [R]", "Simulate [Space]" };

    for (int i = 0; i < 5; ++i) {
        ImGui::PushID(i);
        ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));

        bool isSelected = selectedButton == i;

        // Set the button size to be square and selectable
        if (ImGui::Selectable(selectableLabels[i], isSelected, 0, ImVec2(buttonWidth, 90))) {
            if (i == 4)
            {
                if (isButtonSelected(4))
                {
                    selectedButton = lastSelected;
                    stopSim = true;
                }
                else
                {
					selectedButton = i;
					startSim = true;
				}
			}
			else
			{
                selectedButton = i;
                lastSelected = i;
                stopSim = true;
            }
            
        }

        ImGui::PopID();
        ImGui::PopStyleVar();

        // Separate buttons with a small space
        ImGui::SameLine();
    }

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.0f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.0f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.0f, 0.0f, 1.0f));

    if (ImGui::Button("Delete all", ImVec2(buttonWidth, 90))) {
        delAll = true;
        stopSim = true;
        selectButton(lastSelected);
    }
    ImGui::PopStyleColor(3);

    ImGui::End();
}

bool BWindow::isButtonSelected(int buttonIndex) { return buttonIndex == selectedButton; }


int BWindow::getSelectedButton() { return selectedButton; }

bool BWindow::isWindowFocused() { return isFocused; }

void BWindow::selectButton(int buttonIndex)
{
    selectedButton = buttonIndex;
}
