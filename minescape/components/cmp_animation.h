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
	bool isTitle;
	
public:
	void update(double dt) override;
	void ChangeFrame();
	void FlipSprite(bool flipped);
	void ResetDefaultFrame();
	bool animate = true;
	explicit Animation(Entity* p, std::string filepath, int maxFrame);
	explicit Animation(Entity* p, std::string filepath, int maxFrame, bool title);
	Animation() = delete;
	bool reverse;
};
