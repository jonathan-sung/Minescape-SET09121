#include "cmp_rock.h"

using namespace std;
using namespace sf;

void Rock::update(double dt) {
	_fallTime -= dt;
	bool falling = (getVelocity().y * getVelocity().y) >= 1.0f;
	bool moving = (getVelocity().x * getVelocity().x) >= 4.0f;
	//cout << dt << endl;
	if (!fallTimeElapsed && _fallTime <= 0) {
		fallTimeElapsed = true;
		setCollidable(true);
	}
	if (!moving) {
		_direction = -_direction;
		impulse(_direction * Vector2f(4, 0));
	}
	if (fallTimeElapsed && !falling) impulse(Vector2f(_direction));
	//cout << getVelocity().x << endl;
	PhysicsComponent::update(dt);
	if (_parent->getPosition().y >= 450) _parent->setForDelete();
}

Rock::Rock(Entity* p) : PhysicsComponent(p, true, Vector2f(TILE_SIZE, TILE_SIZE)) {
	_fallTime = (rand() % 5 + 1);
	_direction = Vector2f(0.1f, 0.f);
	setCollidable(false);
	setRestitution(0.0000001f);
	setFriction(0.01f);
	impulse(Vector2f(0.0f, 0.f));
	setMass(1000000000.f);
}
