#include "Game.h"

Entity::Entity(const Animation & anim, float x, float y, float heading) : anim(anim), x(x), y(y), heading(heading) {
}

void Entity::update() {
	x += dx;
	y += dy;
	frame += anim.animRate;
	if (frame >= anim.frames.size()) {
		frame = 0;
		animEnd = true;
	}
	anim.sprite.setTextureRect(anim.frames[static_cast<int>(frame)]);
	anim.sprite.setPosition(x, y);
	anim.sprite.setRotation(heading + 90);
}

void Entity::collisionCheck() {
	//intention of function: optional method checks collisions of one entity against all others
}

bool Entity::collisionHandler(Entity * entity) {
	return false;
}

void Entity::draw(sf::RenderWindow & window) {
	window.draw(anim.sprite);
}

void Entity::wrap() {
	if (x < 0) {
		x = Game::width;
	} else if (x > Game::width) {
		x = 0;
	}
	if (y < 0) {
		y = Game::height;
	} else if (y > Game::height) {
		y = 0;
	}
}

bool Entity::collides(Entity * other) {
	float x = this->x - other->x;
	float y = this->y - other->y;
	float r = this->anim.radius + other->anim.radius;
	return (x * x) + (y * y) < (r * r);
}
