#pragma once

class Ship : public Entity {
public:
	const float MAXSPEED = 15;
	bool thrusting;
	int livesCount;
	sf::Sprite lives;

	Ship(const Animation& anim, float x, float y);
	void update();
	void collisionCheck();
	void drawLives(sf::RenderWindow& window);
};
