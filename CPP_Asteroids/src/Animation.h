#pragma once

class Animation {
public:
	sf::Sprite sprite;
	std::vector<sf::IntRect> frames;
	float animRate;
	float radius;

	Animation();
	Animation(const sf::Texture& texture, int frameCount, int w, int h, int xOffset, int yOffset, float animRate, int oOffset = 0);
};
