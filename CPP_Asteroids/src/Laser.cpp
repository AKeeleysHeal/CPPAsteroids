#include "Game.h"

Laser::Laser(const Animation & anim, float x, float y, float heading, bool enemy) : Entity(anim, x, y, heading), enemy(enemy){
	dx = cos(heading * DEG2RAD) * 6;
	dy = sin(heading * DEG2RAD) * 6;
}

void Laser::update() {
	Entity::update();
	if (x < 0 || x > Game::width || y < 0 || y > Game::height) {
		active = false;
	}
}

void Laser::collisionCheck() {
	if (enemy) return;
	for(Entity* e : Game::curEntities) {
		active = !e->collisionHandler(this);
		if (!active) break;
	}
}

bool Laser::collisionHandler(Entity * entity) {
	if (enemy && collides(entity)) {
		active = false;
		return true;
	}
	return false;
}
