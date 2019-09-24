#pragma once

class Asteroid : public Entity {
public:
	Asteroid(const Animation& anim, float x, float y);
	void update();
	bool collisionHandler(Entity* entity);
};
