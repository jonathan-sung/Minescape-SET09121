#include "cmp_camera.h"
#include <engine.h>

CameraComponent::CameraComponent(Entity* p, const Vector2f& size, const Vector2f& position) :
	Component(p), _size(size)
{
	_offset = Vector2f(size.x / 2.0f, size.y / (2.0f));
	_position = position + _offset;
	_layer = 0;
}

void CameraComponent::update(double dt) 
{
	//check if target above/below max/min y position of layer
	try
	{
		if (_target->getPosition().y < _position.y-_offset.y)
		{
			moveToNextLayer();
		}
		else if (_target->getPosition().y > (_position.y + _offset.y))
		{
			moveToPreviousLayer();
		}
	}
	catch(exception e)
	{
		cout << "Invalid target" << endl;
	}

	//move to layer
	if (_position.y != (_size.y * _layer)+_offset.y)
	{
		_position= Vector2f(_position.x, (_size.y * _layer)+_offset.y);
	}
	
	//set view
	Engine::GetWindow().setView(View(_position,_size));
}

void CameraComponent::setTarget(shared_ptr<Entity> target)
{
	_target = target;
}

void CameraComponent::setLayer(const int layer)
{
	_layer = layer;
}

void CameraComponent::moveToNextLayer()
{
	_layer--;
}

void CameraComponent::moveToPreviousLayer()
{
	_layer++;
}