#include "cmp_rope.h"
#include <engine.h>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <system_physics.h>
#include "cmp_physics.h"
#include <Box2D/Box2D.h>
#include <system_renderer.h>
#include "Box2D/Common/b2Math.h"
#include "LevelSystem.h"
#include "cmp_player_physics.h"
#include "../game.h"

using namespace std;

RopeComponent::RopeComponent(Entity* p,float maxlength,float delay) :Component(p)
{
	buttonPressed = false;
	ropeState = RopeState::Ready;
	ropeMaxLength = maxlength;
	_usable = true;
	_delay = delay;
	launchSpeed = 500;

	impulseTime = 0.001f;
}

void RopeComponent::updateClickPos()
{
	//get window relative mouse coordinates
	Vector2i mousePos = Mouse::getPosition(Engine::GetWindow());
	clickPos = Engine::GetWindow().mapPixelToCoords(mousePos);
}

bool RopeComponent::mouseClick() 
{
	if (Mouse::isButtonPressed(Mouse::Button::Left))
	{
		if (!buttonPressed)
		{
			buttonPressed = true;
			return true;
		}
		else
		{
			return false;
		}
	}
	else 
	{
		if (buttonPressed)
		{
			buttonPressed = false;
		}
		return false;
	}
}

void RopeComponent::setDirectionVector(Vector2f initialPos,Vector2f targetPos)
{
	//set distance to direction vector
	directionVector = Vector2f(
		targetPos.x- initialPos.x,
		targetPos.y- initialPos.y);

	//get the length
	float directionVectorLength = length(directionVector);

	//divide by the length to get size 1 vector
	directionVector = Vector2f(directionVector.x/directionVectorLength,
		directionVector.y/directionVectorLength);
}

bool RopeComponent::isTouchingWall()
{
	if (ls::getTileAt(currentEndPointPosition) == ls::WALL)
		return true;
	else return false;
}

void RopeComponent::createRopeJoint()
{
	//create joint definition
	//set the first body
	auto pyco = _parent->GetCompatibleComponent<PhysicsComponent>();
	_pbody = pyco[0].get()->getFixture()->GetBody();
	_pbody->SetGravityScale(1.0f);
	rjDef.bodyA = _pbody;
	//world vector of the direction of rope launch
	b2Vec2 directionBVec = b2Vec2(directionVector.x, -directionVector.y);
	directionBVec.Normalize();
	//create the second body definition
	b2BodyDef endbodydef;
	//set position to player + direction 
	b2Vec2 offset = b2Vec2(directionBVec.x * (ropeCurrentLength * Physics::physics_scale_inv),
		directionBVec.y * (ropeCurrentLength * Physics::physics_scale_inv));
	endbodydef.position = rjDef.bodyA->GetPosition()+ (offset);
	//make the body static
	endbodydef.type = b2BodyType::b2_staticBody;
	//create second body
	_endBody = Physics::GetWorld()->CreateBody(&endbodydef);
	//set the second body
	rjDef.bodyB = _endBody;

	//make it a rope joint
	rjDef.type = b2JointType::e_ropeJoint;
	rjDef.localAnchorA = b2Vec2(0, 0);
	rjDef.localAnchorB = b2Vec2(0, 0);
	rjDef.collideConnected = false;
	//set the maximum length
	rjDef.maxLength = ropeCurrentLength *Physics::physics_scale_inv;

	//create joint in world
	auto joint = Physics::GetWorld().get()->CreateJoint(&rjDef);
	//set it to instance
	ropeJoint = joint;
}

void RopeComponent::disposeOfDistanceJoint()
{
	try {
		Physics::GetWorld().get()->DestroyJoint(ropeJoint);
	}
	catch (exception e)
	{
		cout << "Rope is already disposed of" << endl;
	}
}

void RopeComponent::update(double dt)
{
	if (!_usable)
	{
		if (ropeState == RopeState::Latched)
		{
			disposeOfDistanceJoint();
			ropeState = RopeState::Withdrawing;
		}
		else
		{
			delaytimer = _delay;
			ropeState = RopeState::Unusable;
		}
		auto pl = _parent->get_components<PlayerPhysicsComponent>()[0];
		if (pl != NULL)
		{
			_usable = pl.get()->isCollidable();
		}
	}
	switch (ropeState)
	{
		//rope is ready to be used
	case RopeState::Ready:
	{
		//prepare for rope firing animation
		currentEndPointPosition = _parent->getPosition();

		//on mouse click
		if (mouseClick())
		{
			//handle calculation for rope firing
			updateClickPos();
			//set the direction vector
			setDirectionVector(_parent->getPosition(), clickPos);


			//fire rope
			ropeState = RopeState::InAir;
		}
		break;
	}
	case RopeState::InAir:
	{
		initialRopePosition = _parent->getPosition();

		if (length(currentEndPointPosition - initialRopePosition) < ropeMaxLength)
		{
			currentEndPointPosition += Vector2f(directionVector.x * launchSpeed * dt,
				directionVector.y * launchSpeed * dt);
		}
		else ropeState = RopeState::Withdrawing;

		if (isTouchingWall())
		{
			cout << "touched" << endl;
			finalRopePosition = currentEndPointPosition;
			ropeCurrentLength = length(finalRopePosition - initialRopePosition);
			//create the rope joint
			createRopeJoint();

			forceImpulseVelocity = Vector2f(0.0f, 0.0f);
			addedImpulseAcceleration = Vector2f(0.0f, 0.0f);
			swingTimer = 0;

			ropeState = RopeState::Latched;
		}

		break;
	}
	case RopeState::Latched:
	{
		initialRopePosition = _parent->getPosition();

		//not touching the ground
		if (!_parent->get_components<PlayerPhysicsComponent>()[0].get()->isGrounded()) {
			if (impulseTimer < 0) {
				impulseTimer = impulseTime;
				//get the direction vector
				Vector2f directionVector = Vector2f(finalRopePosition.x - _parent->getPosition().x,
					finalRopePosition.y - _parent->getPosition().y);
				directionVector = normalize(directionVector);
				Vector2f directionVectorPerpendicular = Vector2f(-directionVector.x,
					(directionVector.y < 0 ? -directionVector.y : directionVector.y));
				//get the angle
				float angle = asin(directionVector.y / (sqrt(pow(directionVector.x, 2) + pow(directionVector.y, 2))));
				//take 90 degrees
				angle += b2_pi / 2;
				//get atan
				float angleATan = atan(angle);
				cout << "angle" << angleATan << endl;
				//create added impulse
				if (angleATan < 1) {

					if (angleATan < 0.5f)angleATan = 0.5f;
					else if (angleATan > 0.7f)angleATan = 0.7f;

					if (swingTimer <= 0) {
						if (Keyboard::isKeyPressed(Engine::keyControls[Engine::keybinds::Left]))
						{
							if (directionVector.x < 0)
							{
								addedImpulseAcceleration += Vector2f(-directionVectorPerpendicular.x  * dt * 500,
									directionVectorPerpendicular.y  * dt * 500);
							}
							else
							{
								addedImpulseAcceleration += Vector2f(directionVectorPerpendicular.x  * dt * 500,
									-directionVectorPerpendicular.y  * dt * 500);
							}
							swingTimer = 0.05f;
						}
						else if (Keyboard::isKeyPressed(Engine::keyControls[Engine::keybinds::Right]))
						{
							if (directionVector.x > 0)
							{
								addedImpulseAcceleration += Vector2f(-directionVectorPerpendicular.x  * dt * 500,
									directionVectorPerpendicular.y  * dt * 500);
							}
							else
							{
								addedImpulseAcceleration += Vector2f(directionVectorPerpendicular.x  * dt * 500,
									-directionVectorPerpendicular.y  * dt * 500);
							}
							swingTimer = 0.05f;
						}
					}
					swingTimer -= dt;


					addedImpulseAcceleration += Vector2f(
						dt * -directionVectorPerpendicular.x * angleATan * 10,
						dt * -directionVectorPerpendicular.y * -angleATan * 10);

					if (addedImpulseAcceleration.x < -ropeCurrentLength )addedImpulseAcceleration.x = -ropeCurrentLength ;
					else if (addedImpulseAcceleration.x > ropeCurrentLength )addedImpulseAcceleration.x = ropeCurrentLength ;
				
					if (addedImpulseAcceleration.y < -ropeCurrentLength)addedImpulseAcceleration.y = -ropeCurrentLength;
					else if (addedImpulseAcceleration.y > ropeCurrentLength)addedImpulseAcceleration.y = ropeCurrentLength;


					cout << "directionVectorPerpendicular: " << directionVectorPerpendicular.x << " " <<
						directionVectorPerpendicular.y << endl;

					forceImpulseVelocity += addedImpulseAcceleration;

					if (forceImpulseVelocity.x < -ropeCurrentLength * 3)forceImpulseVelocity.x = -ropeCurrentLength * 3;
					else if (forceImpulseVelocity.x > ropeCurrentLength * 3)forceImpulseVelocity.x = ropeCurrentLength * 3;
					if (forceImpulseVelocity.y > ropeCurrentLength * 1.1f)forceImpulseVelocity.y = ropeCurrentLength * 1.1f;
					else if (forceImpulseVelocity.y < -ropeCurrentLength / 5)forceImpulseVelocity.y = ropeCurrentLength;

					cout << "impulse" << forceImpulseVelocity.x << " " <<
						forceImpulseVelocity.y << endl;
					_parent->get_components<PlayerPhysicsComponent>()[0].get()->impulse(forceImpulseVelocity);

				}
				else 
				{
					//get the direction vector
					Vector2f directionVector = Vector2f(finalRopePosition.x - _parent->getPosition().x,
						finalRopePosition.y - _parent->getPosition().y);
					directionVector = normalize(directionVector);
					Vector2f directionVectorPerpendicular = Vector2f(-directionVector.x,
						(directionVector.y < 0 ? -directionVector.y : directionVector.y));

					forceImpulseVelocity = Vector2f(directionVectorPerpendicular.x*ropeCurrentLength, ropeCurrentLength*1.5f);
					addedImpulseAcceleration = Vector2f(forceImpulseVelocity.x*0.75f, forceImpulseVelocity.y*0.75f);
				}
			}
			else
			{
				impulseTimer -= dt;
			}
		}

		//once latched make it controllable by player
		if (Keyboard::isKeyPressed(Keyboard::Space))
		{
			cout << "changing states" << endl;
			forceImpulseVelocity = Vector2f(forceImpulseVelocity.x, -forceImpulseVelocity.y);
			_parent->get_components<PlayerPhysicsComponent>()[0].get()->impulse(forceImpulseVelocity);
			disposeOfDistanceJoint();
			ropeState = RopeState::Withdrawing;
		}
		//when player presses jump button go to withdraw and make player jump
		break;
	}
	case RopeState::Withdrawing:
	{
		initialRopePosition = _parent->getPosition();
		setDirectionVector(_parent->getPosition(), currentEndPointPosition);

		if (length(currentEndPointPosition - initialRopePosition) > 10.0f)
		{
			currentEndPointPosition += Vector2f(-directionVector.x * launchSpeed * dt,
				-directionVector.y * launchSpeed * dt);
		}
		else
		{
			//once it has fully reduced go to unusable
			delaytimer = _delay;
			ropeState = RopeState::Unusable;
		}

		break;
	}
	case RopeState::Unusable:
	{
		//stay here for a bit (cooldown?)
		delaytimer -= dt;
		if (delaytimer <= 0)
		{
			//go back to ready
			ropeState = RopeState::Ready;
			//reset mouse click for reuse
			buttonPressed = false;
		}
		break;
	}
	default:
		break;
	}
}

void RopeComponent::render() 
{
	//only render if rope is out
	if (ropeState == RopeState::InAir || ropeState == RopeState::Latched || ropeState == RopeState::Withdrawing) {
		
		//create vertex array (this should be a temporary solution)
		sf::Vertex ropeLines[] = { sf::Vertex(initialRopePosition),
			sf::Vertex(currentEndPointPosition)};
		//render rope
		Engine::GetWindow().draw( ropeLines , 2, sf::Lines);
	}
}

void RopeComponent::setUsable(bool usable)
{
	_usable = usable;
}
