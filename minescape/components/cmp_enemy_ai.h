#pragma once
#include "cmp_actor_movement.h"
#include <ecm.h>

class EnemyAIComponent : public ActorMovementComponent {
protected:

private:


public:
	void update(double dt) override;
	void render() override;

	explicit EnemyAIComponent(Entity* p);
	EnemyAIComponent() = delete;
};