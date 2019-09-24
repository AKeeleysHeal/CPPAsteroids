#include "Game.h"

Explosion::Explosion(const Animation& anim, float x, float y) : Entity(anim, x, y, 0) {
}

void Explosion::update() {
	Entity::update();
	if (animEnd) active = false;
}
