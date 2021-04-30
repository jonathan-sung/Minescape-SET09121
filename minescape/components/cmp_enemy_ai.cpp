#include "cmp_enemy_ai.h"

using namespace std;
using namespace sf;

void EnemyAIComponent::update(double dt) {

	switch (state)
	{
	case EnemyAIComponent::Waiting: {
		//checks player within vision radius
		sf::Vector2f distanceToPlayerVec2f = _parent->getPosition() - _player->getPosition();
		float distanceToPlayer = length(distanceToPlayerVec2f);
		if (distanceToPlayer < _visionRadius)
		{
			//decide direction to move
			if ((_parent->getPosition().x < _player->getPosition().x && _speed.x < 0) ||
				(_parent->getPosition().x > _player->getPosition().x && _speed.x > 0))
			{
				_speed.x = -_speed.x;
			}

			state = State::PlayerDetected;
		}
		break;
	}
	case EnemyAIComponent::PlayerDetected: {
		_initialPos = _parent->getPosition();
		state = State::Moving;
		break;
	}
	case EnemyAIComponent::Moving: {
		sf::Vector2f mov = sf::Vector2f(_speed.x * dt, _speed.y * dt) + _parent->getPosition();

		if ((mov.y < _parent->getPosition().y - _movementRadius.y) ||
			(mov.y > _parent->getPosition().y + _movementRadius.y))
		{
			_speed.y = -_speed.y;
		}

		if (validMove(mov)) {
			move(sf::Vector2f(_speed.x * dt, _speed.y * dt));

			ActorMovementComponent::update(dt);
		}
		else
		{
			waitTimeTick = waitTime / 2;
			state = State::PauseAfterMove;
		}

		//if reaches x momevement radius
		float distanceToStartPoint = length(_parent->getPosition() - _initialPos);
		if (distanceToStartPoint > _movementRadius.x)
		{
			waitTimeTick = waitTime;
			state = State::PauseAfterMove;
		}

		break;
	}
	case EnemyAIComponent::PauseAfterMove: {
		waitTimeTick -= dt;
		if (waitTimeTick <= 0)
		{
			state = State::Returning;
		}
		break;
	}
	case EnemyAIComponent::Returning: {
		sf::Vector2f distanceToInitialVector = _initialPos - _parent->getPosition();

		if (length(distanceToInitialVector) < 0.5f)
		{
			state = State::Waiting;
		}
		else
		{
			sf::Vector2f sizeOneVector = normalize(distanceToInitialVector);
			move(sf::Vector2f(sizeOneVector.x * dt, sizeOneVector.y * dt));
		}

		break;
	}
	default:
		break;
	}

}

EnemyAIComponent::EnemyAIComponent(Entity* p, Entity* player, float visionRadius, sf::Vector2f speed, sf::Vector2f movementRadius, float wait):
	ActorMovementComponent(p)
{
	_player = player;
	_visionRadius = visionRadius;
	_speed = speed;
	_movementRadius = movementRadius;
	waitTime = wait;

	_initialPos = p->getPosition();
	waitTimeTick = waitTime;

	state = State::Waiting;
}