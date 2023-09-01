#include "imgui.h"
#include "imgui-SFML.h"
#include "SFML/Graphics.hpp"

#include "drawingHandler.h"
#include "windows/bWindow.h"
#include "windows/jointWindow.h"
#include "joint.h"
#include "stick.h"

#include <iostream>
#include <vector>

using namespace ImGui;
using namespace sf;

namespace Constants
{
	constexpr int WIDTH = 1200;
	constexpr int HEIGHT = 800;

	constexpr int lineThickness = 4;
	constexpr int dotSpacing = 10;
	constexpr int radius = 15;
	constexpr int stiffnes = 50;

	constexpr float deltaTime = 0.1f;
    const sf::Vector2f gravity = sf::Vector2f(0, 9.81f);

    const sf::Color darkGray = Color(30, 34, 45);
    const sf::Color gray = Color(45, 50, 60);
    const sf::Color white = Color(250, 250, 250);

}

bool openedB = true;
bool delAll = false;
bool stopSim = false;
bool startSim = false;
int lastSelected = 0;
void handleInput(const sf::RenderWindow& window, const sf::Event& event, BWindow& bWindow);

std::vector<Stick*> sticks;
std::vector<Joint*> joints;
void updateAll(const std::vector<Stick*>& sticks, const std::vector<Joint*>& joints, float deltaTime, sf::Vector2f gravity);
void checkDeletion(std::vector<Stick*>& sticks, std::vector<Joint*>& joints);
void deleteAll(std::vector<Stick*>& sticks, std::vector<Joint*>& joints);

bool simulating = false;
std::vector<Stick*> oldSticks;
std::vector<Joint*> oldJoints;
void startSimulation();
void stopSimulation();

Joint* selectedJoint = nullptr;

int main()
{
	RenderWindow window(VideoMode(Constants::WIDTH, Constants::HEIGHT), "Window Title");
	SFML::Init(window);


    Clock deltaClock;
    BWindow bWindow;

 //   for (int i = 0; i < 50; i++) {
	//	joints.push_back(new Joint(sf::Vector2f(400 + i * 5, 200), Constants::white));
	//}
 //   for (int i = 0; i < 49; i++) {
 //       sticks.push_back(new Stick(joints[i], joints[i + 1], Constants::white));
 //   }

    while (window.isOpen())
    {
        sf::Event event{};
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            handleInput(window, event, bWindow);
            if (event.type == sf::Event::Closed)
                window.close();
        }
        ImGui::SFML::Update(window, deltaClock.restart());

        if (openedB)
        {
            bWindow.update(delAll, lastSelected, stopSim, startSim);
        }
        if (selectedJoint)
        {
            JointWindow jointWindow(selectedJoint);
            jointWindow.update(simulating);
        }
        if (simulating)
        {
            updateAll(sticks, joints, Constants::deltaTime, Constants::gravity);
		}
        else
        {
            checkDeletion(sticks, joints);
        }
        if (delAll)
        {
			deleteAll(sticks, joints);
		}
        if (stopSim)
        {
			stopSimulation();
            stopSim = !stopSim;
		}
        if (startSim)
        {
			startSimulation();
			startSim = !startSim;
		}

        window.clear(sf::Color(18, 33, 43)); // Color background
        DrawingHandler::drawBackground(window, Constants::HEIGHT, Constants::WIDTH, Constants::lineThickness, Constants::dotSpacing, Constants::gray);
        ImGui::SFML::Render(window);

        DrawingHandler::drawSticks(window, sticks, Constants::lineThickness);
        DrawingHandler::drawJoints(window, joints, Constants::radius);

        window.display();
    }
    // Cleanup memory
    for (auto joint : joints)
    {
        delete joint;
    }
    for (auto stick : sticks)
    {
        delete stick;
    }


    ImGui::SFML::Shutdown();
    return 0;
}

void handleInput(const sf::RenderWindow& window, const sf::Event& event, BWindow& bWindow)
{
    if (event.type == sf::Event::KeyPressed)
    {
        switch (event.key.code)  
        {
           case sf::Keyboard::B:
               openedB = !openedB;
			break;
           case sf::Keyboard::Q:
                bWindow.selectButton(0);
                lastSelected = 0;
                stopSim = true;
            break;
           case sf::Keyboard::W:
				bWindow.selectButton(1);
                lastSelected = 1;
				stopSim = true;
            break;
           case sf::Keyboard::E:
                bWindow.selectButton(2);
                lastSelected = 2;
                stopSim = true;
            break;
           case sf::Keyboard::R:
				bWindow.selectButton(3);
                lastSelected = 3;
                stopSim = true;
            break;
           case sf::Keyboard::Space:
               if (bWindow.isButtonSelected(4))
               {
                   bWindow.selectButton(lastSelected);
                   stopSim = true;
               }
			   else
               {
                   bWindow.selectButton(4);
                   startSim = true;
               }

			break;
           case sf::Keyboard::Escape:
               selectedJoint = nullptr;
			break;
           case sf::Keyboard::Delete:
               if (selectedJoint && !simulating)
               {
                   selectedJoint->deleted = true;
               }
            break;
           case sf::Keyboard::F:
               if (selectedJoint && !simulating)
               {
				   selectedJoint->fixed = !selectedJoint->fixed;
			   }
			break;

        default:
            break;
        }
    }

    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            if (!bWindow.isWindowFocused())
            {
	            const sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (bWindow.isButtonSelected(0))
                {
                    for (const auto& joint : joints)
                    {
	                    const sf::Vector2f jointPos = joint->position;
	                    const float distance = std::sqrt((mousePos.x - jointPos.x) * (mousePos.x - jointPos.x) +
                            (mousePos.y - jointPos.y) * (mousePos.y - jointPos.y));
                        if (distance <= Constants::radius)
                        {
                            selectedJoint = joint;
                            break;
                        }
					}
                }

                if (bWindow.isButtonSelected(1))
                {
                    if (!joints.empty())
                    {
                        bool found = false;
                        if (selectedJoint)
                        {
	                        const float selectedDistance = std::sqrt((mousePos.x - selectedJoint->position.x) * (mousePos.x - selectedJoint->position.x) +
                                (mousePos.y - selectedJoint->position.y) * (mousePos.y - selectedJoint->position.y));
                            if (selectedDistance >= Constants::radius)
                            {
                                for (const auto& joint : joints)
                                {
                                    if (joint == selectedJoint) break;
                                    const sf::Vector2f jointPos = joint->position;
                                    const float distance = std::sqrt((mousePos.x - jointPos.x) * (mousePos.x - jointPos.x) +
                                        (mousePos.y - jointPos.y) * (mousePos.y - jointPos.y));
                                    if (distance <= Constants::radius * 2)
                                    {
                                        sticks.push_back(new Stick(joint, selectedJoint, Constants::white));
                                        found = true;
                                        selectedJoint = joint;
                                        break;
                                    }
                                }
                                if (!found) {
                                    joints.push_back(new Joint(sf::Vector2f(mousePos.x, mousePos.y), Constants::white));
                                    sticks.push_back(new Stick(joints[joints.size() - 1], selectedJoint, Constants::white));
                                    selectedJoint = joints[joints.size() - 1];
                                }
                            }
                        }
                        else
                        {
                            for (const auto& joint : joints)
                            {
	                            const sf::Vector2f jointPos = joint->position;
	                            const float distance = std::sqrt((mousePos.x - jointPos.x) * (mousePos.x - jointPos.x) +
                                    (mousePos.y - jointPos.y) * (mousePos.y - jointPos.y));
                                if (distance <= Constants::radius * 2)
                                {
                                    selectedJoint = joint;
                                    found = true;
                                    break;
                                }
                            }
                            if (!found) {
                                joints.push_back(new Joint(sf::Vector2f(mousePos.x, mousePos.y), Constants::white));
                                selectedJoint = joints[joints.size() - 1];
                            }
                        }
                    }
                    else
                    {
						joints.push_back(new Joint(sf::Vector2f(mousePos.x, mousePos.y), Constants::white));
						selectedJoint = joints[joints.size() - 1];
					}
                }

                if (bWindow.isButtonSelected(3))
                {
                    for (const auto& joint : joints)
                    {
	                    const sf::Vector2f jointPos = joint->position;
	                    const float distance = std::sqrt((mousePos.x - jointPos.x) * (mousePos.x - jointPos.x) +
                            (mousePos.y - jointPos.y) * (mousePos.y - jointPos.y));
                        if (distance <= Constants::radius)
                        {
	                        joint->deleted = true;
                            break;
                        }
                    }
                }
            }
		}
	}
}


void updateAll(const std::vector<Stick*>& sticks, const std::vector<Joint*>& joints, const float deltaTime,
               const sf::Vector2f gravity)
{
    for (const auto joint : joints)
    {
	    joint->update(deltaTime, gravity);
	}
    for (int i = 0; i < Constants::stiffnes; i++)
    {
        for (const auto stick : sticks)
        {
	        stick->update();
        }
    }
}

void checkDeletion(std::vector<Stick*>& sticks, std::vector<Joint*>& joints)
{
    std::vector<Joint*> jointsToDelete;
    for (auto& joint : joints)
    {
        if (joint->deleted) jointsToDelete.push_back(joint);
    }
    for (Joint* joint : jointsToDelete) {
        for (int i = 0; i < sticks.size(); i++)
        {
            if (sticks[i]->jointA == joint || sticks[i]->jointB == joint)
            {
                sticks.erase(sticks.begin() + i);
                i--;
            }
        }
        joints.erase(std::find(joints.begin(), joints.end(), joint));
        selectedJoint = nullptr;
    }
    jointsToDelete.clear();
}

void deleteAll(std::vector<Stick*>& sticks, std::vector<Joint*>& joints)
{
    for (const auto& stick : sticks)
    {
		delete stick;
	}
    for (const auto& joint : joints)
    {
		delete joint;
	}
	sticks.clear();
	joints.clear();
	selectedJoint = nullptr;
	simulating = false;
    delAll = false;
}

void startSimulation()
{
    simulating = true;
    oldSticks.clear();
    oldJoints.clear();
    bool selected = false;
    
    for (const auto& stick : sticks)
    {
        Joint* jointA = nullptr;
        Joint* jointB = nullptr;
        for (const auto& oldJoint : oldJoints)
        {
            if (oldJoint->position == stick->jointA->position)
            {
                jointA = oldJoint;
                break;
            }
        }
        for (const auto& oldJoint : oldJoints)
        {
            if (oldJoint->position == stick->jointB->position)
            {
                jointB = oldJoint;
                break;
            }
        }
        if (jointA && jointB)
        {
            oldSticks.push_back(new Stick(jointA, jointB, stick->color));
        }
        else if (jointA)
        {
            oldJoints.emplace_back(jointB = new Joint(stick->jointB->position, stick->jointB->color, stick->jointB->fixed));
            oldSticks.push_back(new Stick(jointA, jointB, stick->color));
        }
        else if (jointB)
        {
            oldJoints.emplace_back(jointA = new Joint(stick->jointA->position, stick->jointA->color, stick->jointA->fixed));
            oldSticks.push_back(new Stick(jointA, jointB, stick->color));
        }
        else
        {
            oldJoints.emplace_back(jointA = new Joint(stick->jointA->position, stick->jointA->color, stick->jointA->fixed));
            oldJoints.emplace_back(jointB = new Joint(stick->jointB->position, stick->jointB->color, stick->jointB->fixed));
            oldSticks.push_back(new Stick(jointA, jointB, stick->color));
        }
        if (selectedJoint && selectedJoint->position == stick->jointA->position)
        {
            selectedJoint = jointA;
            selected = true;
        }
        else if (selectedJoint && selectedJoint->position == stick->jointB->position)
        {
            selectedJoint = jointB;
            selected = true;
        }
    }

    for (const auto& joint : joints)
    {
        bool found = false;
        for (const auto& oldJoint : oldJoints)
        {
            if (joint->position == oldJoint->position)
            {
                found = true;
				break;
			}
		}
        if (!found)
        oldJoints.emplace_back(new Joint(joint->position, joint->color, joint->fixed));
        if (selectedJoint && selectedJoint->position == joint->position && !selected)
        {
			selectedJoint = oldJoints[oldJoints.size() - 1];
            selected = true;
        }
	}
}

void stopSimulation()
{
    if (simulating)
    {
        simulating = false;
        sticks.clear();
        joints.clear();
        sticks = oldSticks;
        joints = oldJoints;
    }
    
}
