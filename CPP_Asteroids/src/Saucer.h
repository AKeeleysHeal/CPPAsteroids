#pragma once

class Saucer : public Entity {
public:
	Saucer(const Animation& anim, float x, float y);
	void update();
	bool collisionHandler(Entity* entity);
	void destroy();
};
