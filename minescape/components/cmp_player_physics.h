#pragma once

#include "cmp_physics.h"

class PlayerPhysicsComponent : public PhysicsComponent {
protected:
  b2Vec2 _size;
  sf::Vector2f _maxVelocity;
  bool _grounded;
  float _groundspeed;

  bool stunned;
  float stun_time;
  void stunning(double dt);
  bool playerInWall();

public:
  void update(double dt) override;
  
  void stun();

  bool isGrounded() const;
  explicit PlayerPhysicsComponent(Entity* p, const sf::Vector2f& size);

  PlayerPhysicsComponent() = delete;
};
