#include "cmp_canary_ai.h"
#include <engine.h>

using namespace std;
using namespace sf;

CanaryAIComponent::CanaryAIComponent(Entity* p, float visionRadius, float wait, sf::Vector2f speed,sf::Vector2f movementRect) :
	EnemyAIComponent(p),
	_visionRadius(visionRadius), waitTime(wait), waitTimeTick(waitTime), 
	_speed(speed), _movementRect(movementRect)
{
	_player = _parent->scene->ents.find("player")[0];
	_initialPos = p->getPosition();
	_direction = sf::Vector2f(1, 1);
	state = State::Waiting;
	turnedLeft = false;
}

void CanaryAIComponent::update(double dt) {
	try {
		if (auto pl = _player.lock()) {
			switch (state)
			{
			case CanaryAIComponent::Waiting: {
				//checks player within vision radius
				sf::Vector2f distanceToPlayerVec2f = _parent->getPosition() - pl->getPosition();
				float distanceToPlayer = length(distanceToPlayerVec2f);
				if (distanceToPlayer < _visionRadius)
				{
					//decide direction to move
					if ((_parent->getPosition().x < pl->getPosition().x && _direction.x < 0) ||
						(_parent->getPosition().x > pl->getPosition().x && _direction.x > 0))
					{
						_direction.x = -_direction.x;
						if (_direction.x > 0) turnedLeft = false;
						else turnedLeft = true;
					}

					state = State::PlayerDetected;
				}
				break;
			}
			case CanaryAIComponent::PlayerDetected: {
				_initialPos = _parent->getPosition();
				angle = 0;
				state = State::Moving;
				break;
			}
			case CanaryAIComponent::Moving: {

				if (angle > 90 || angle < -90)
				{
					_direction.y = -_direction.y;
				}
				angle += _direction.y*dt*3;

				sf::Vector2f mov = sf::Vector2f(_speed.x * dt*_direction.x, sin(angle)*_speed.y * dt*_direction.y) + _parent->getPosition();

				if (!validMove(mov)) {
					waitTimeTick = waitTime / 2;
					state = State::PauseAfterMove;
				}
				else
				{
					move(sf::Vector2f(_speed.x * dt*_direction.x, sin(angle) * _speed.y * dt * _direction.y));

					//ActorMovementComponent::update(dt);

					//if reaches x momevement radius
					float distanceToStartPoint = length(_parent->getPosition() - _initialPos);
					if (distanceToStartPoint > _movementRect.x)
					{
						waitTimeTick = waitTime;
						state = State::PauseAfterMove;
					}
				}

				break;
			}
			case CanaryAIComponent::PauseAfterMove: {
				waitTimeTick -= dt;
				if (waitTimeTick <= 0)
				{
					turnedLeft = -turnedLeft;
					state = State::Returning;
				}
				break;
			}
			case CanaryAIComponent::Returning: {
				sf::Vector2f distanceToInitialVector = _initialPos - _parent->getPosition();

				if (length(distanceToInitialVector) < 0.5f)
				{
					state = State::Waiting;
				}
				else
				{
					sf::Vector2f unitVector = normalize(distanceToInitialVector);
					move(sf::Vector2f(unitVector.x * dt* _speed.x, unitVector.y * dt* _speed.y));
				}

				break;
			}
			default:
				break;
			}
		}
	}
	catch(exception e)
	{
		cout << "CanaryAIComponent::update::ERROR" << endl;
		return;
	}
}

void CanaryAIComponent::render()
{
}

bool CanaryAIComponent::isTurnedLeft()
{
	return turnedLeft;
}
