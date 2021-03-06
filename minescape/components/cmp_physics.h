#pragma once

#include "cmp_sprite.h"
#include "ecm.h"
#include <Box2D/Dynamics/b2Body.h>

class PhysicsComponent : public Component {
protected:
  b2Body* _body;
  bool _dynamic;
  b2Fixture* _fixture;
  bool iscollidable;

public:
  PhysicsComponent(Entity* p, bool dyn, const sf::Vector2f& size);

  b2Fixture* const getFixture() const;
  bool isTouching(const PhysicsComponent& pc) const;
  bool isTouching(const PhysicsComponent& pc, b2Contact const* bc) const;
  std::vector<const b2Contact*> getTouching() const;
  const sf::Vector2f getVelocity() const;
  void setRestitution(float r);
  void setFriction(float r);
  void setMass(float m);
  void setDynamic(bool b);
  void setCollidable(bool b);
  bool isCollidable();
  void update(double dt) override;
  void render() override;
  void impulse(const sf::Vector2f& i);
  void dampen(const sf::Vector2f& s);
  void setVelocity(const sf::Vector2f& v);
  void teleport(const sf::Vector2f& v);
  ~PhysicsComponent() override;
};
