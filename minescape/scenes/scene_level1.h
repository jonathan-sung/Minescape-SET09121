#pragma once

#include "engine.h"

class Level1Scene : public Scene {
public:
  void Load() override;

  void UnLoad() override;

  void Update(const double& dt) override;

  void togglePause();

  void Render() override;

  std::vector<sf::Vector2ul> ar;
};
