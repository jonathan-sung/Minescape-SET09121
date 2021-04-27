#pragma once

#include "cmp_physics.h"

class PlayerPhysicsComponent : public PhysicsComponent {
protected:
  b2Vec2 _size;
  sf::Vector2f _maxVelocity;
  bool _grounded;
  float _groundspeed;

  bool isGrounded() const;
  bool stunned;
  float stun_time;
  void stunning(double dt);

public:
  void update(double dt) override;
  
  void stun();

  explicit PlayerPhysicsComponent(Entity* p, const sf::Vector2f& size);

  PlayerPhysicsComponent() = delete;
};
