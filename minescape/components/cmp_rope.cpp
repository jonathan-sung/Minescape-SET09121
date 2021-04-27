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

using namespace std;

RopeComponent::RopeComponent(Entity* p,float maxlength,float delay) :Component(p)
{
	buttonPressed = false;
	ropeState = RopeState::Ready;
	ropeMaxLength = maxlength;
	_usable = true;
	_delay = delay;
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
	if (ls::getTileAt(finalRopePosition) == ls::WALL)
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
	b2Vec2 offset = b2Vec2(directionBVec.x * (ropeMaxLength * Physics::physics_scale_inv),
		directionBVec.y * (ropeMaxLength * Physics::physics_scale_inv));
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
	rjDef.collideConnected = true;
	//set the maximum length
	rjDef.maxLength = ropeMaxLength*Physics::physics_scale_inv;

	//create joint in world
	auto joint = Physics::GetWorld().get()->CreateJoint(&rjDef);
	//set it to instance
	ropeJoint = joint;
}

void RopeComponent::disposeOfDistanceJoint()
{
	Physics::GetWorld().get()->DestroyJoint(ropeJoint);
	//Physics::GetWorld().get()->DestroyBody(ropeJoint->GetBodyB());
	
}

void RopeComponent::updateRopePoints()
{
	initialRopePosition = _parent->getPosition();

	//set the endpoint to the bodyB point
	Vector2f dir = Vector2f(
		ropeJoint->GetBodyB()->GetPosition().x - ropeJoint->GetBodyA()->GetPosition().x,
		ropeJoint->GetBodyB()->GetPosition().y - ropeJoint->GetBodyA()->GetPosition().y
	);
	dir = Vector2f(dir.x * Physics::physics_scale,
		-dir.y * Physics::physics_scale);
	finalRopePosition = _parent->getPosition() + dir;
}

void RopeComponent::update(double dt)
{
	if (!_usable)
	{
		if (ropeState == RopeState::Latched) ropeState = RopeState::Withdrawing;
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
		//on mouse click
		if (mouseClick())
		{
			//handle calculation for rope firing
			updateClickPos();
			//set the direction vector
			setDirectionVector(_parent->getPosition(), clickPos);
			//create the rope joint
			createRopeJoint();

			//fire rope
			ropeState = RopeState::InAir;
		}
		break;
	}
	case RopeState::InAir:
	{
		updateRopePoints();
		if (isTouchingWall())
		{
			cout << "touched" << endl;
			ropeState = RopeState::Latched;
		}
		else ropeState = RopeState::Withdrawing;
		//once it touched a wall tile go to latch
		//if (isTouchingWall()) ropeState = RopeState::Latched;
		//else ropeState = RopeState::Withdrawing;

		break;
	}
	case RopeState::Latched:
	{
		updateRopePoints();
		//once latched make it controllable by player
		if (Keyboard::isKeyPressed(Keyboard::Space))
		{
			cout << "changing states" << endl;
			ropeState = RopeState::Withdrawing;
		}
		//when player presses jump button go to withdraw and make player jump
		break;
	}
	case RopeState::Withdrawing:
	{
		disposeOfDistanceJoint();

		//once it has fully reduced go to unusable
		delaytimer = _delay;
		ropeState = RopeState::Unusable;

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
			sf::Vertex(finalRopePosition)};
		//render rope
		Engine::GetWindow().draw( ropeLines , 2, sf::Lines);
	}
}

void RopeComponent::setUsable(bool usable)
{
	_usable = usable;
}
