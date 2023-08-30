#pragma once

#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "../joint.h"

class JointWindow
{
private:
	Joint* joint;
public:
	JointWindow(Joint* joint);
	void update(bool simulating);
};