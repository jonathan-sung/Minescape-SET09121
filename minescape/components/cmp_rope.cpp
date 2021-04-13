#include "cmp_rope.h"
#include <engine.h>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/Vertex.hpp>
using namespace std;


RopeComponent::RopeComponent(Entity* p) :Component(p) 
{
	buttonPressed = false;
	ropeState = RopeState::Ready;
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

void RopeComponent::updateDirectionVector()
{
	//set distance to direction vector
	directionVector = Vector2f(
		clickPos.x- _parent->getPosition().x,
		clickPos.y- _parent->getPosition().y);

	//get the length
	float directionVectorLength = length(directionVector);
	cout << directionVectorLength << endl;
	//divide by the length to get size 1 vector
	directionVector = Vector2f(directionVector.x/directionVectorLength,
		directionVector.y/directionVectorLength);
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
			updateDirectionVector();
			//fire rope
			ropeState = RopeState::InAir;
		}
		break;
	}
	case RopeState::InAir: 
	{
		//move point in increment until it touches a tile

		//once it touched a wall tile go to latch

		//if its not a wall tile go to withdraw
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
		//shrink back to size

		//once it has fully reduced go to unusable
		break;
	}
	case RopeState::Unusable: 
	{
		//stay here for a bit (cooldown?)
		break;
	}
	default:
		break;
	}
}

void RopeComponent::render() 
{
	//only render if outside
	if (ropeState == RopeState::InAir || ropeState == RopeState::Latched || ropeState == RopeState::Withdrawing) {
		sf::Vertex line[] =
		{
			sf::Vertex(_parent->getPosition()),
			sf::Vertex(clickPos)
		};
		Engine::GetWindow().draw(line, 2, sf::Lines);
	}
}