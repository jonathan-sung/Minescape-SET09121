#include "cmp_hurt_player.h"
#include <engine.h>
#include "cmp_player_physics.h"
#include "cmp_rope.h"

using namespace std;
using namespace sf;

void HurtComponent::update(double dt) {
	if (auto pl = _player.lock()) {
		if (length(pl->getPosition() - _parent->getPosition()) < 64.0) {
			pl->get_components<PlayerPhysicsComponent>()[0]->stun();
			pl->get_components<RopeComponent>()[0]->setUsable(false);
			cout << "Stunned" << endl;
			//pl->setForDelete();
			_parent->setForDelete();
		}
	}
}

HurtComponent::HurtComponent(Entity* p)
	: Component(p), _player(_parent->scene->ents.find("player")[0]) {}
