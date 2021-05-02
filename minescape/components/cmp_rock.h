#pragma once

#include "ecm.h"
#include "cmp_physics.h"
#include <stdlib.h>

#define TILE_SIZE 64.0f
#define DEFAULT_DIRECTION_TIME 2
#define MAX_SPEED 200
#define LIFE_TIME 30

class Rock : public PhysicsComponent {
protected:
	float _fallTime;
	bool fallTimeElapsed = false;
	sf::Vector2f _direction;
	float changeDirectionTime;
	float pushing_counter;

public:
	void update(double dt) override;

	explicit Rock(Entity* p);

	Rock() = delete;
};
