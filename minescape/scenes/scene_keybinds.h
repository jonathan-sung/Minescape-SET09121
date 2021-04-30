#pragma once

#include "engine.h"

using namespace std;
using namespace sf;

class KeybindsScene : public Scene {
private:
	bool rebinding;
	int selection;
	//static float buttonCD;
	shared_ptr<Entity> options[7];

	void RebindKey(int index);

public:
	KeybindsScene() = default;
	~KeybindsScene() override = default;

	void Load() override;

	void Update(const double& dt) override;

	void changeText();
};
