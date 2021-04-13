#pragma once
#include "ecm.h"

using namespace sf;

class RopeComponent : public Component {
private:
	Vector2f clickPos;
	Vector2f directionVector;
	bool buttonPressed;

	enum RopeState
	{
		Ready,
		InAir,
		Latched,
		Withdrawing,
		Unusable
	};

	RopeState ropeState;

	void updateClickPos();
	bool mouseClick();
	void updateDirectionVector();
	
public:

	explicit RopeComponent(Entity* p);
	RopeComponent() = delete;

	void update(double dt) override;
	void render() override;
};
