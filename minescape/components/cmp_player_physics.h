#pragma once

#include "cmp_physics.h"
#include <SFML/Audio.hpp>

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
  sf::SoundBuffer stunSoundBuffer;
  sf::SoundBuffer jumpSoundBuffer;

  sf::Sound stunSound;
  sf::Sound jumpSound;

public:
  void update(double dt) override;
  
  void stun();

  bool isGrounded() const;
  explicit PlayerPhysicsComponent(Entity* p, const sf::Vector2f& size);

  PlayerPhysicsComponent() = delete;
};
