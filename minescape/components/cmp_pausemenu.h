#pragma once
#include "engine.h"
#include "cmp_text.h"

using namespace std;
using namespace sf;

class PauseMenu : public Component
{
private:
	static int selection;
	shared_ptr<TextComponent> options[3];
	static float delta;
public:
	explicit PauseMenu(Entity* p);
	PauseMenu() = delete;

	void update(double dt) override;
	void render() override;

	void pauseGame();
	void changeText();
};