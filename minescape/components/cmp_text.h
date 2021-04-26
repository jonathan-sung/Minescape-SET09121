#pragma once

#include <SFML/Graphics/Text.hpp>
#include <ecm.h>

using namespace sf;

class TextComponent : public Component {
public:
  TextComponent() = delete;
  explicit TextComponent(Entity* p, const std::string& str = "");
  void update(double dt) override;

  void setPosition(Vector2f pos);

  void render() override;

  ~TextComponent() override = default;

  void SetText(const std::string& str);

protected:
  std::shared_ptr<sf::Font> _font;
  std::string _string;
  sf::Text _text;
};
