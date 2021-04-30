#pragma once

#include "engine.h"

using namespace std;
using namespace sf;

class KeybindsScene : public Scene {
private:
	bool rebinding;
	int selection;
	shared_ptr<Entity> options[7];
public:
	KeybindsScene() = default;
	~KeybindsScene() override = default;

	void Load() override;

	void Update(const double& dt) override;

	void changeText();
};
