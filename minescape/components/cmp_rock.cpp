#include "cmp_rock.h"
#include "cmp_animation.h"

using namespace std;
using namespace sf;

void Rock::update(double dt) {
	_fallTime -= dt;

	bool falling = (getVelocity().y * getVelocity().y) >= 1.0f;
	bool moving = abs(getVelocity().x) > 0;
	//cout << getVelocity().x << endl;
	if (!fallTimeElapsed && _fallTime <= 0) {
		fallTimeElapsed = true;
		setCollidable(true);
	}

	if (abs(getVelocity().x) == 0) {
		pushing_counter += dt;
		if (pushing_counter >= DEFAULT_DIRECTION_TIME + ((rand() % 10)) / 10) {
			_direction = -_direction;
			pushing_counter = 0;
		}
	}

	if (fallTimeElapsed && !falling && abs(getVelocity().x) < MAX_SPEED) {
		impulse(Vector2f(_direction));
		_parent->get_components<Animation>()[0]->getSprite().rotate(_direction.x * 10);
		//setVelocity(_direction);
	}

	//cout << getVelocity().x << endl;
	PhysicsComponent::update(dt);
	if (_parent->getPosition().y >= 450 || _fallTime <= -LIFE_TIME) _parent->setForDelete();
}

Rock::Rock(Entity* p) : PhysicsComponent(p, true, Vector2f(TILE_SIZE, TILE_SIZE)) {
	_fallTime = (rand() % 5 + 1);
	_direction = Vector2f(0.1f, 0.f);
	setCollidable(false);
	setRestitution(0.0000001f);
	setFriction(0.01f);
	impulse(Vector2f(0.0f, 0.f));
	setMass(1000000000.f);
	changeDirectionTime = 2;
	pushing_counter = 0;
}
