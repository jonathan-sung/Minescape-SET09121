#pragma once

#include "scenes/scene_level1.h"
#include "scenes/scene_level2.h"
#include "scenes/scene_level3.h"
#include "scenes/scene_menu.h"
#include "scenes/scene_options.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Joystick.hpp>

extern bool enterDown;
extern const sf::Keyboard::Key keyControls[6];
extern enum keybinds 
{
	Up = 0,
	Left = 1,
	Down = 2,
	Right = 3,
	Action1 = 4,
	Action2 = 5
};
//extern const sf::Joystick::Axis contControls[6];


extern MenuScene menu;
extern Level1Scene level1;
extern OptionsScene optionscene;


//extern Level2Scene level2;
//extern Level3Scene level3;
