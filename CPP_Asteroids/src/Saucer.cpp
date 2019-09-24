#include "Game.h"

Saucer::Saucer(const Animation& anim, float x, float y) : Entity(anim, x, y, 0) {
	Game::sounds["saucer"].play();
	dx = 2 + randf(3);
	dy = 1 + randf(2);
	if (std::rand() % 2) {
		dx *= -1;
		x = Game::width;
	}
	if (std::rand() % 2) {
		dy *= -1;
	}
}

void Saucer::update() {
	Entity::update();
	if (x < 0 || x > Game::width || y < 0 || y > Game::height) {
		active = false;
	}
	else if (std::rand() % 150 == 0) { 
		float heading = atan2(Game::ship->x - x, y - Game::ship->y) / DEG2RAD - 90;
		Game::entities.push_back(new Laser(Game::animations["laserSauce"]), x, y, heading, true));
	}
}

bool Saucer::collisionHandler(Entity * entity) {
	if (collides(entity)) {
		active = false;
		Game::entities.push_back(new Explosion(Game::animations["explosionShip"], x, y));
		Game::score += 200;
		Game::sounds["bigBang"].play();
		return true;
	}
	return false;
}

void Saucer::destroy() {
	active = false;
	Game::saucer = nullptr;
	Game::sounds["saucer"].stop();
}
