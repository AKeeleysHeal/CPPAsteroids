#pragma once

class Laser : public Entity {
public:
	Laser(const Animation& anim, float x, float y, float heading, bool enemy = false);
	void update();
	void collisionCheck();
	bool collisionHandler(Entity* entity);
private: 
	bool enemy;
};
