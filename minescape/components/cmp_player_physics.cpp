#include "cmp_player_physics.h"
#include "system_physics.h"
#include <LevelSystem.h>
#include <SFML/Window/Keyboard.hpp>
#include "../components/cmp_animation.h"
#include "../game.h"

using namespace std;
using namespace sf;
using namespace Physics;

#define STUN_TIME 1
#define LEFT_EDGE 2 * ls::getTileSize() + 20
#define RIGHT_EDGE (ls::getWidth() - 3) * ls::getTileSize() - 20

bool PlayerPhysicsComponent::isGrounded() const {
	auto touch = getTouching();
	const auto& pos = _body->GetPosition();
	const float halfPlrHeigt = _size.y * .5f;
	const float halfPlrWidth = _size.x * .52f;
	b2WorldManifold manifold;
	for (const auto& contact : touch) {
		contact->GetWorldManifold(&manifold);
		const int numPoints = contact->GetManifold()->pointCount;
		bool onTop = numPoints > 0;
		// If all contacts are below the player.
		for (int j = 0; j < numPoints; j++) {
			onTop &= (manifold.points[j].y < pos.y - halfPlrHeigt);
		}
		if (onTop) {
			return true;
		}
	}

	return false;
}

void PlayerPhysicsComponent::update(double dt) {

	const auto pos = _parent->getPosition();

	//Teleport to start if we fall off map.
	if (pos.y > ls::getHeight() * ls::getTileSize()) {
		teleport(ls::getTilePosition(ls::findTiles(ls::START)[0]));
	}
	//Teleport if too close to edge
	if (pos.x > RIGHT_EDGE) {
		teleport(Vector2f(RIGHT_EDGE, _parent->getPosition().y));
	}
	else if (pos.x < LEFT_EDGE) {
		//_parent->setPosition(Vector2f(2 * ls::getTileSize(), _parent->getPosition().y));
		teleport(Vector2f(LEFT_EDGE, _parent->getPosition().y));
		//teleport(ls::getTilePosition(ls::findTiles(ls::START)[0]));
	}
	//if (Keyboard::isKeyPressed(Keyboard::S)) stun();
	_parent->get_components<Animation>()[0]->animate = false;
	if (!stunned) {
		if (Engine::keyPressed[Engine::keybinds::Left]||
			Engine::keyPressed[Engine::keybinds::Right]) {
			_parent->get_components<Animation>()[0]->animate = true;
			// Moving Either Left or Right
			if (Engine::keyPressed[Engine::keybinds::Right]) {
				_parent->get_components<Animation>()[0]->FlipSprite(true);
				if (getVelocity().x < _maxVelocity.x)
					impulse({ (float)(dt * _groundspeed), 0 });
			}
			else {
				_parent->get_components<Animation>()[0]->FlipSprite(false);
				if (getVelocity().x > -_maxVelocity.x)
					impulse({ -(float)(dt * _groundspeed), 0 });
			}
		}
		else {
			// Dampen X axis movement
			dampen({ 0.3f, 1.0f });
			//_parent->get_components<Animation>()[0]->ResetDefaultFrame();
		}


		if (Engine::keyPressed[Engine::keybinds::Left] ||
			Engine::keyPressed[Engine::keybinds::Right]) {
			// Moving Either Left or Right
			_parent->get_components<Animation>()[0]->animate = true;
			if (Engine::keyPressed[Engine::keybinds::Right]) {
				_parent->get_components<Animation>()[0]->FlipSprite(true);
				if (getVelocity().x < _maxVelocity.x)
					impulse({ (float)(dt * _groundspeed), 0 });
			}
			else {
				_parent->get_components<Animation>()[0]->FlipSprite(false);
				if (getVelocity().x > -_maxVelocity.x)
					impulse({ -(float)(dt * _groundspeed), 0 });
			}
		}
		else {
			// Dampen X axis movement
			dampen({ 0.3f, 1.0f });
		}

		// Handle Jump
		if (Engine::keyPressed[Engine::keybinds::Action1]|| Engine::keyPressed[Engine::keybinds::Up]) {
			_grounded = isGrounded();
			if (_grounded) {
				setVelocity(Vector2f(getVelocity().x, 0.f));
				teleport(Vector2f(pos.x, pos.y - 2.0f));
				impulse(Vector2f(0, -10.f));
				SoundEngine::fx_jump.play();
			}
		}
	}
	//Are we in air?
	if (!_grounded) {
		// Check to see if we have landed yet
		_grounded = isGrounded();
		// disable friction while jumping
		setFriction(0.f);
	}
	else {
		setFriction(0.1f);
	}

	//ask jonathan about this
		// Clamp velocity.
	auto v = getVelocity();
	v.x = copysign(min(abs(v.x), _maxVelocity.x), v.x);
	v.y = copysign(min(abs(v.y), _maxVelocity.y), v.y);
	setVelocity(v);
	if (stunned) stunning(dt);
	PhysicsComponent::update(dt);
}

void PlayerPhysicsComponent::stun() {
	if (!stunned) {
		stun_time = STUN_TIME;
		stunned = true;
		setCollidable(false);
		SoundEngine::fx_stun.play();
	}
}

void PlayerPhysicsComponent::stunning(double dt) {
	if (stunned) {
		stun_time -= dt;
		if (stun_time <= 0 && !playerInWall()) {
			stunned = false;
			setCollidable(true);
		}
	}
}

bool PlayerPhysicsComponent::playerInWall() {
	return 	getTouching().size() != 0;
}

PlayerPhysicsComponent::PlayerPhysicsComponent(Entity* p,
	const Vector2f& size)
	: PhysicsComponent(p, true, size) {
	_size = sv2_to_bv2(size, true);
	_maxVelocity = Vector2f(200.f, 400.f);
	_groundspeed = 50.f;
	_grounded = false;
	_body->SetSleepingAllowed(false);
	_body->SetFixedRotation(true);
	//Bullet items have higher-res collision detection
	_body->SetBullet(true);
	stun_time = STUN_TIME;
	stunned = false;
}
