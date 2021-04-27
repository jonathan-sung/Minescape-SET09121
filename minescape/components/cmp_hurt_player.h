#pragma once
#include "cmp_physics.h"
#include <ecm.h>
#include <SFML/Audio.hpp>

class HurtComponent : public Component {
protected:
  std::weak_ptr<Entity> _player;

public:
  void update(double dt) override;
  void render() override {}
  explicit HurtComponent(Entity* p);
  HurtComponent() = delete;
  sf::SoundBuffer buffer;
  sf::Sound sound;
};
