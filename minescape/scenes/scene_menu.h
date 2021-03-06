#pragma once

#include "engine.h"

using namespace std;
using namespace sf;

class MenuScene : public Scene {
private:
	int selection;
	shared_ptr<Entity> options[3];

public:
	MenuScene() = default;
	~MenuScene() override = default;

	void Load() override;

	void UnLoad() override;

	void stopMenuMusic();

	void Update(const double& dt) override;

	void changeText();
};
