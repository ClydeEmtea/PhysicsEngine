#pragma once

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
    bool isButtonSelected(int buttonIndex) const;
    int getSelectedButton() const;
    bool isWindowFocused() const;

};
