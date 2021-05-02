#pragma once

#include "engine.h"
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;

class MenuScene : public Scene {
private:
	int selection;
	shared_ptr<Entity> options[3];
	Music music;

public:
	MenuScene() = default;
	~MenuScene() override = default;

	void Load() override;
	void UnLoad() override;
	void Update(const double& dt) override;

	void changeText();
};
