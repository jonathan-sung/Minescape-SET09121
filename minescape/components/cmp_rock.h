#pragma once

#include "ecm.h"
#include "cmp_physics.h"
#include <stdlib.h>

#define TILE_SIZE 64.0f

class Rock : public PhysicsComponent {
protected:
	float _fallTime;
	bool fallTimeElapsed = false;
	sf::Vector2f _direction;

public:
	void update(double dt) override;

	explicit Rock(Entity* p);

	Rock() = delete;
};
