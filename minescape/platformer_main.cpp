#include "engine.h"
#include "game.h"
#include "scenes/scene_menu.h"
#include "SFML/Graphics.hpp"

using namespace std;
using namespace sf;

bool enterDown;
bool paused = false;
float buttonCD;

//#############
//Levels
Level1Scene level1;

//#############
//Menus
MenuScene menu;
OptionsScene optionscene;
KeybindsScene keyBindScene;

/*
Level2Scene level2;
Level3Scene level3;
*/


int main() {

    ////set initial keybinds
    //Engine::keyControls[Engine::keybinds::Up] = Keyboard::W;
    //Engine::keyControls[Engine::keybinds::Down] = Keyboard::S;
    //Engine::keyControls[Engine::keybinds::Left] = Keyboard::A;
    //Engine::keyControls[Engine::keybinds::Right] = Keyboard::D;
    //Engine::keyControls[Engine::keybinds::Action1] = Keyboard::Space;
    //Engine::keyControls[Engine::keybinds::Action2] = Keyboard::Enter;
	Engine::Start(1280, 720, "Minescape", &menu);
}