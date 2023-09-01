#pragma once

#include "../joint.h"

class JointWindow
{
private:
	Joint* joint;
public:
	explicit JointWindow(Joint* joint);
	void update(bool simulating) const;
};