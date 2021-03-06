#include "cmp_animation.h"
using namespace std;

void Animation::update(double dt) {
	//getSprite().setTexture()
	if (animate) {
		currentTime += dt;
		if (currentTime >= changeFrameTime) {
			currentTime = 0;
			if (reverse) {
				currentFrame--;
			}
			else {
				currentFrame++;
			}
			if (currentFrame >= maxFrame) {
				currentFrame = 0;
			}
			else if (currentFrame < 0) {
				currentFrame = maxFrame- 1;
			}
			if (isTitle) {
				getSprite().setTextureRect(sf::IntRect(currentFrame * 512, 0 * TILE_SIZE, 512, TILE_SIZE));
			}
			else {
				ChangeFrame();
			}

		}
	}

	SpriteComponent::update(dt);
}

void Animation::ChangeFrame() {
	getSprite().setTextureRect(sf::IntRect(currentFrame * TILE_SIZE, 0 * TILE_SIZE, TILE_SIZE, TILE_SIZE));
}

void Animation::FlipSprite(bool flipped) {
	if (flipped) {
		getSprite().setScale(-1, 1);
	}
	else {
		getSprite().setScale(1, 1);
	}

}

void Animation::ResetDefaultFrame()
{
	currentFrame = 0;
}

Animation::Animation(Entity* p, string filepath, int maxFrame) : SpriteComponent(p) {
	currentFrame = 0;
	currentTime = 0;
	reverse = false;
	isTitle = false;
	this->maxFrame = maxFrame;
	animate = true;
	if (!spritesheet.loadFromFile(filepath)) {
		cerr << "Failed to load spritesheet!" << std::endl;
	}
	getSprite().setTextureRect(sf::IntRect(0 * TILE_SIZE, 0 * TILE_SIZE, TILE_SIZE, TILE_SIZE));
	getSprite().setOrigin(sf::Vector2f((TILE_SIZE / 2), TILE_SIZE / 2));
	getSprite().setTexture(spritesheet);

}

Animation::Animation(Entity* p, string filepath, int maxFrame, bool title) : SpriteComponent(p) {
	currentFrame = 0;
	currentTime = 0;
	reverse = false;
	isTitle = title;
	this->maxFrame = maxFrame;
	animate = true;
	if (!spritesheet.loadFromFile(filepath)) {
		cerr << "Failed to load spritesheet!" << std::endl;
	}
	getSprite().setTextureRect(sf::IntRect(0 * 512, 0 * TILE_SIZE, 512, TILE_SIZE));
	getSprite().setOrigin(sf::Vector2f((512 / 2), TILE_SIZE / 2));
	getSprite().setTexture(spritesheet);

}


