#pragma once

#include "ecm.h"
#include "../components/cmp_sprite.h"
#include <SFML/Graphics.hpp>
#include "../game.h"
#include <string>

#define TILE_SIZE 64.0f

class Animation : public SpriteComponent {
protected:
	int currentFrame;
	int maxFrame;
	const float changeFrameTime = 0.1f;
	float currentTime;
	sf::Texture spritesheet;
	
public:
	void update(double dt) override;
	void ChangeFrame();
	void FlipSprite(bool flipped);
	bool animate = true;
	explicit Animation(Entity* p, std::string filepath, int maxFrame);
	Animation() = delete;
};
