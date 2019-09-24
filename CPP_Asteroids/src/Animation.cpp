#include "Game.h"

Animation::Animation() : animRate(0), radius(0) {}

Animation::Animation(const sf::Texture & texture, int frameCount, int w, int h, int xOffset, int yOffset, float animRate, int oOffset) : 
	animRate(animRate), radius(w / 2)
{
	sprite.setTexture(texture);
	for (int i = 0; i < frameCount; i++) {
		frames.push_back(sf::IntRect(xOffset + (w * i), yOffset, w, h));
	}
	sprite.setOrigin(static_cast<float>(w * 0.5), static_cast<float>(h * 0.5 + oOffset));
}
