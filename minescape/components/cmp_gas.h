#pragma once
#include "cmp_actor_movement.h"
#include <ecm.h>

class GasComponent : public ActorMovementComponent
{
protected:
	sf::Vector2f _direction;
private:
	float _speed;
public:
	explicit GasComponent(Entity* p);
	GasComponent() = delete;
	void update(double dt) override;

};