#pragma once

class Explosion : public Entity {
public:
	Explosion(const Animation& anim, float x, float y);
	void update();
};
