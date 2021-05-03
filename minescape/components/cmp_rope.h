#pragma once

#include "ecm.h"
#include <Box2D/Box2D.h>
#include <SFML/Audio.hpp>

using namespace sf;

class RopeComponent : public Component {
private:
	Vector2f clickPos;
	Vector2f directionVector;
	bool buttonPressed;

	float launchSpeed;
	float ropeCurrentLength;
	float ropeMaxLength;
	float impulseTime;
	float impulseTimer;
	Vector2f forceImpulse; 
	Vector2f addedImpulse;

	Vector2f initialRopePosition;
	Vector2f currentEndPointPosition;
	Vector2f finalRopePosition;
	
	b2Body* _endBody;
	b2Fixture* _endfixture;
	b2Body* _pbody;
	b2Fixture* _pfixture;
	b2RopeJointDef rjDef;
	b2Joint* ropeJoint;

	bool _usable;
	float _delay;
	float delaytimer;
	sf::Sound rope_shoot_sound;
	sf::Sound rope_hit_sound;
	sf::SoundBuffer buffer_shoot;
	sf::SoundBuffer buffer_hit;


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
	void setDirectionVector(Vector2f initialPos, Vector2f targetPos);
	bool isTouchingWall();
	void createRopeJoint();
	void disposeOfJoint();
	
public:

	explicit RopeComponent(Entity* p,float maxlength, float delay);
	RopeComponent() = delete;

	void update(double dt) override;
	void render() override;

	void setUsable(bool usable);
};
