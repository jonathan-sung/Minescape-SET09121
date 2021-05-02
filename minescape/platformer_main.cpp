#include "engine.h"
#include "game.h"
#include "scenes/scene_menu.h"
#include "SFML/Graphics.hpp"

using namespace std;
using namespace sf;

bool enterDown;
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

sf::Keyboard::Key keyControls[6];

int main() {

    //set initial keybinds
    keyControls[keybinds::Up] = Keyboard::W;
    keyControls[keybinds::Left] = Keyboard::A;
    keyControls[keybinds::Down] = Keyboard::S;
    keyControls[keybinds::Right] = Keyboard::D;
    keyControls[keybinds::Action1] = Keyboard::Space;
    keyControls[keybinds::Action2] = Keyboard::Enter;
	Engine::Start(1280, 720, "Minescape", &menu);
}