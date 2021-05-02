#pragma once
#include <ecm.h>
#include "cmp_enemy_ai.h"

class CanaryAIComponent : public EnemyAIComponent {
protected:
	std::weak_ptr<Entity> _player;
	float _visionRadius;
	sf::Vector2f _initialPos;
	sf::Vector2f _speed;
	sf::Vector2f _direction;
	sf::Vector2f _movementRect;
	float waitTime;
	float waitTimeTick;
	float angle;

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
	void render() override;

	explicit CanaryAIComponent(Entity* p, float visionRadius, float wait,sf::Vector2f speed,sf::Vector2f movementRect);
	CanaryAIComponent() = delete;
};