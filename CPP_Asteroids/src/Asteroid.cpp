#include "Game.h"

Asteroid::Asteroid(const Animation & anim, float x, float y) : Entity(anim, x, y, randf(360)) {
	dx = 1 + randf(2);
	dy = 1 + randf(2);
	if (std::rand() % 2) dx *= -1;
	if (std::rand() % 2) dy *= -1;
}

void Asteroid::update() {
	Entity::update();
	Entity::wrap();
}

bool Asteroid::collisionHandler(Entity * entity) {
	if (active && collides(entity)) {
		active = false;
		Game::entities.push_back(new Explosion(Game::animations["explosionAsteroid"], x, y));
		if (anim.radius > 30) {
			Game::entities.push_back(new Asteroid(Game::animations["asteroidMd"], x, y));
			Game::entities.push_back(new Asteroid(Game::animations["asteroidMd"], x, y));
			Game::sounds["bigBang"].play();
			Game::score += 25;
		} else if (anim.radius > 20) {
			Game::entities.push_back(new Asteroid(Game::animations["asteroidSm"], x, y));
			Game::entities.push_back(new Asteroid(Game::animations["asteroidSm"], x, y));
			Game::sounds["medBang"].play();
			Game::score += 50;
		} else {
			Game::sounds["smallBang"].play();
			Game::score += 75;
		}
		return true;
	}
	return false;
}
