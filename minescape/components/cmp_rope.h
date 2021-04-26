#pragma once
#include "ecm.h"

using namespace sf;

class RopeComponent : public Component {
private:
	Vector2f clickPos;
	Vector2f directionVector;
	bool buttonPressed;

	float launchSpeed;
	float ropeMaxLength;

	Vector2f initialRopePosition;
	Vector2f finalRopePosition;

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
	void setRopePoints(Vector2f initialPoint, Vector2f finalPoint);
	Vector2f updatedRopePoint(const double dt);
	void setDirectionVector(Vector2f initialPos, Vector2f targetPos);
	float getRopeCurrentLength();
	bool isTouchingWall();
	
public:

	explicit RopeComponent(Entity* p,float launchspeed,float maxlength);
	RopeComponent() = delete;

	void update(double dt) override;
	void render() override;
};
