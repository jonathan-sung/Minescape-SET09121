#pragma once

#include "scenes/scene_level1.h"
#include "scenes/scene_level2.h"
#include "scenes/scene_level3.h"
#include "scenes/scene_menu.h"
#include "scenes/scene_options.h"
#include "scenes/scene_keybinds.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Joystick.hpp>

extern bool enterDown;

extern bool paused;

extern float buttonCD;

extern sf::Keyboard::Key keyControls[6];


extern enum keybinds 
{
	Up = 0,
	Left = 2,
	Down = 1,
	Right = 3,
	Action1 = 4,
	Action2 = 5
};

extern float musicVolume;
extern float sfxVolume;

extern vector<Vector2f> resolutions{
Vector2f(800,600),
Vector2f(1280,720),
Vector2f(1920,1080),
Vector2f(3840,2160)
};

extern bool windowed;
extern bool Gamepad;

//############################################
//Levels
extern Level1Scene level1;

//############################################
//Menus
extern MenuScene menu;
extern OptionsScene optionscene;
extern KeybindsScene keyBindScene;


//extern Level2Scene level2;
//extern Level3Scene level3;
