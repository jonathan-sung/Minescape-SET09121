#pragma once
#include <ecm.h>

using namespace sf;
using namespace std;

class CameraComponent : public Component
{
protected:
	Vector2f _size;
	Vector2f _position;
	Vector2f _offset;
	shared_ptr<Entity> _target;
	int _layer;
	const sf::Vector2f targetResolution=sf::Vector2f(1920,1080);

public:
	explicit CameraComponent(Entity* p,const Vector2f& size,const Vector2f& position);
	CameraComponent() = delete;

	void update(double dt) override;
	void render() override {}

	void setTarget(shared_ptr<Entity> target);
	void setLayer(const int layer);

private:
	void moveToNextLayer();
	void moveToPreviousLayer();
};