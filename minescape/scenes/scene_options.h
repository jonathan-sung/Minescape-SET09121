#pragma once

#include "engine.h"

using namespace std;
using namespace sf;

class OptionsScene : public Scene {
private:

	int selection;
	shared_ptr<Entity> options[3];
public:
	OptionsScene() = default;
	~OptionsScene() override = default;

	void Load() override;

	void Update(const double& dt) override;

	void changeText();
};
