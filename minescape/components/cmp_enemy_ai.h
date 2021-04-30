#pragma once

#include "cmp_actor_movement.h"

using namespace sf;

class EnemyAIComponent : public ActorMovementComponent {
protected:
	Entity* _player;
	float _visionRadius;
	sf::Vector2f _initialPos;
	sf::Vector2f _speed;
	sf::Vector2f _movementRadius;
	float waitTime;
	float waitTimeTick;

private:
	enum State {
		Waiting,
		PlayerDetected,
		Moving,
		PauseAfterMove,
		Returning
	};

	State state;


public:
	void update(double dt) override;

	explicit EnemyAIComponent(Entity* p, Entity* player, float visionRadius, sf::Vector2f speed, sf::Vector2f movementRadius, float wait);

	EnemyAIComponent() = delete;
};