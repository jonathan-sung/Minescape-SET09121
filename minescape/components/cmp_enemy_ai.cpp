#include "cmp_enemy_ai.h"

using namespace std;
using namespace sf;

void EnemyAIComponent::update(double dt)
{
}

void EnemyAIComponent::render()
{
}

EnemyAIComponent::EnemyAIComponent(Entity* p) : ActorMovementComponent(p) {
	_speed = 100.0f;
}
