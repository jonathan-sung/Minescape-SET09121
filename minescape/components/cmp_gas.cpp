#include "cmp_gas.h"
#include "engine.h"
#include "../scenes/scene_menu.h"
#include "../game.h"

using namespace std;
using namespace sf;



void GasComponent::update(double dt)
{
		//Move the Gas up at slow pace
		auto pos = _parent->getPosition();
		_parent->setPosition(Vector2f(pos.x, pos.y + (dt * _speed)));
		
		//If player goes below the Gas, kill the player
		if (pos.y < _parent->scene->ents.find("player")[0]->getPosition().y - 25) 
		{
			cout << "BE DEAD NOW!!!" << endl;
			Engine::GetWindow().setView(View(Vector2f(640, 360), Vector2f(1280, 720)));
			Engine::ChangeScene(&menu);
		}
		
		ActorMovementComponent::update(dt);
}

GasComponent::GasComponent(Entity* p) : ActorMovementComponent(p)
{
	_direction = Vector2f(0, -1.0f);
	_speed = -25.0f;
}

