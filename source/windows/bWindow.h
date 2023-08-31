#pragma once

#include <SFML/Graphics.hpp>
#include "imgui.h"

class BWindow {
private:
    int selectedButton;
    ImVec2 windowPos;
    bool isBeingDragged;
    bool isFocused;

public:
    BWindow();

    void update(bool& delAll, int& lastSelected, bool& stopSim, bool& startSim);
    void selectButton(int buttonIndex);
    bool isButtonSelected(int buttonIndex);
    int getSelectedButton();
    bool isWindowFocused();

};
