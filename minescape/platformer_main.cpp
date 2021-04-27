#include "engine.h"
#include "game.h"
#include "scenes/scene_menu.h"

using namespace std;
using namespace sf;

const Keyboard::Key keyControls[6] =
{
	Keyboard::W,
	Keyboard::A,
	Keyboard::S,
	Keyboard::D,
	Keyboard::Space,
	Keyboard::Enter
};



bool enterDown;

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
  Engine::Start(1280, 720, "Minescape",&menu);
}