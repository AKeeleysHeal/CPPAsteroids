#pragma once

class Entity {
public:
	float x, y; //position
	float dx = 0, dy = 0;
	float heading;
	bool active = true;
	bool animEnd = false;
	Animation anim;
	float frame = 0; 
	
	Entity(const Animation& anim, float x, float y, float heading);
	virtual void update();
	virtual void collisionCheck();
	virtual bool collisionHandler(Entity* entity);
	void draw(sf::RenderWindow& window);
	void wrap();
	bool collides(Entity* other);

};