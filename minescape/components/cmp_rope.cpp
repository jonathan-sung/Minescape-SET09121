#include "cmp_rope.h"
#include <engine.h>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <system_physics.h>
using namespace std;

RopeComponent::RopeComponent(Entity* p, float launchspeed,float maxlength) :Component(p)
{
	buttonPressed = false;
	ropeState = RopeState::Ready;
	launchSpeed = launchspeed;
	ropeMaxLength = maxlength;
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

void RopeComponent::setRopePoints(Vector2f initialPoint, Vector2f finalPoint)
{
	initialRopePosition= initialPoint;
	finalRopePosition = finalPoint;
}

Vector2f RopeComponent::updatedRopePoint(const double dt)
{
	finalRopePosition += Vector2f(directionVector.x*dt*launchSpeed,
		directionVector.y*dt*launchSpeed);
	return finalRopePosition;
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

float RopeComponent::getRopeCurrentLength()
{
	float currRopeLength = length(finalRopePosition- initialRopePosition);
	return currRopeLength;
}

bool RopeComponent::isTouchingWall()
{
	b2BodyDef BodyDef;
	// Is Dynamic(moving), or static(Stationary)
	BodyDef.type = b2_staticBody;
	BodyDef.position = b2Vec2(finalRopePosition.x,finalRopePosition.y);

	// Create the body
	b2Body* _body;
	_body = Physics::GetWorld()->CreateBody(&BodyDef);
	_body->SetActive(true);
	{
		// Create the fixture shape
		b2PolygonShape Shape;
		// SetAsBox box takes HALF-Widths!
		Shape.SetAsBox(0.1f, 0.1f);
		b2FixtureDef FixtureDef;
		// Fixture properties
		FixtureDef.density = 0.0f;
		FixtureDef.friction = 0.0f;
		FixtureDef.restitution = 0.0f;
		FixtureDef.shape = &Shape;
	}
	//const auto _otherFixture = pc.getFixture();
	const auto& w = *Physics::GetWorld();
	const auto contactList = w.GetContactList();
	const auto clc = w.GetContactCount();
	for (int32 i = 0; i < clc; i++) {
		const auto& contact = (contactList[i]);
		cout << "checking contacts at: " << to_string(_body->GetPosition().x) <<
			"x " << to_string(_body->GetPosition().y) << "y" << endl;
		if (contact.IsTouching())
		{
			cout << to_string(contact.GetFixtureA()->GetType()) << endl;
		}
	}
	return false;
}

void RopeComponent::update(double dt)
{
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
			setDirectionVector(_parent->getPosition(), clickPos);
			setRopePoints(_parent->getPosition(), _parent->getPosition());

			//fire rope
			ropeState = RopeState::InAir;
		}
		break;
	}
	case RopeState::InAir:
	{
		//move point in increment until it touches a tile
		setRopePoints(_parent->getPosition(), updatedRopePoint(dt));

		//once it touched a wall tile go to latch
		if (isTouchingWall())
			ropeState = RopeState::Latched;

		//if it reacher max length go to withdraw
		if (getRopeCurrentLength() >= ropeMaxLength)
			ropeState = RopeState::Withdrawing;

		break;
	}
	case RopeState::Latched:
	{
		//once latched make it controllable by player

		//when player presses jump button go to withdraw and make player jump
		break;
	}
	case RopeState::Withdrawing:
	{
		//constant update of direction vector, in case player is moving
		setDirectionVector(finalRopePosition ,_parent->getPosition());

		//shrink back to size
		setRopePoints(_parent->getPosition(), updatedRopePoint(dt));

		//once it has fully reduced go to unusable
		if (getRopeCurrentLength() <= 20)
			ropeState = RopeState::Unusable;

		break;
	}
	case RopeState::Unusable:
	{
		//stay here for a bit (cooldown?)
		//go back to ready
		ropeState = RopeState::Ready;
		//reset mouse click for reuse
		buttonPressed = false;

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
			sf::Vertex(finalRopePosition) };
		//render rope
		Engine::GetWindow().draw( ropeLines , 2, sf::Lines);
	}
}