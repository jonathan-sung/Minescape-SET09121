#pragma once

#include "scenes/scene_level1.h"
#include "scenes/scene_level2.h"
#include "scenes/scene_level3.h"
#include "scenes/scene_menu.h"
#include "scenes/scene_options.h"
#include "scenes/scene_keybinds.h"
#include "scenes/scene_score.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Joystick.hpp>
#include "sound_engine.h"

extern bool enterDown;

extern bool paused;

extern float buttonCD;


//############################################
//Levels
extern Level1Scene level1;

//############################################
//Menus
extern MenuScene menu;
extern OptionsScene optionscene;
extern KeybindsScene keyBindScene;
extern ScoreScene score_board;


//extern Level2Scene level2;
//extern Level3Scene level3;
