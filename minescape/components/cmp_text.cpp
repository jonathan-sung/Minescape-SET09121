#include "cmp_text.h"
#include <system_renderer.h>
#include <system_resources.h>

void TextComponent::update(double dt) {}

void TextComponent::render() { Renderer::queue(&_text); }

TextComponent::TextComponent(Entity* const p, const std::string& str)
	: Component(p), _string(str) {
	_text.setString(_string);
	_font = Resources::get<sf::Font>("TR.ttf");
	_text.setFont(*_font);
}

void TextComponent::SetText(const std::string& str) {
	_string = str;
	_text.setString(_string);
}

void TextComponent::setPosition(Vector2f pos)
{
	_text.setPosition(pos);
}

Vector2f TextComponent::getPosition()
{
    return _text.getPosition();
}

void TextComponent::SetFont(std::shared_ptr<sf::Font> font)
{
	_font = font;
}

void TextComponent::SetSize(int size)
{
	_text.setCharacterSize(size);
}

int TextComponent::GetSize() {
	return _text.getCharacterSize();
}
