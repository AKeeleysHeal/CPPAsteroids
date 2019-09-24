#include "Game.h"

Ship::Ship(const Animation & anim, float x, float y) : Entity(anim, x, y, 0), thrusting(false), livesCount(2) {
	lives = Game::animations["shipStill"].sprite;
	lives.setTextureRect(Game::animations["shipStill"].frames[0]);
	lives.setRotation(0);
	lives.setScale(0.5, 0.5);
}

void Ship::update() {
	if (thrusting) {
		this->anim = Game::animations["shipMove"];
		
		dx += cos(heading * DEG2RAD) * 0.2f;
		dy += sin(heading * DEG2RAD) * 0.2f;
		if (Game::sounds["thrust"].getStatus() == sf::Sound::Stopped) {
			Game::sounds["thrust"].play();
		}
	} else {
		dx *= 0.97f;
		dy *= 0.97f;
		this->anim = Game::animations["shipStill"];
		Game::sounds["thrust"].stop();
	}
	float speed = sqrt(dx * dx + dy * dy);
	if (speed > MAXSPEED) {
		dx *= MAXSPEED / speed;
		dy *= MAXSPEED / speed;
	}
	Entity::update();
	Entity::wrap();
}

void Ship::collisionCheck() {
	for(Entity* e : Game::curEntities) {
		if (e->collisionHandler(this)) {
			Game::entities.push_back(new Explosion(Game::animations["explosionShip"], x, y));
			Game::sounds["bigBang"].play();
			x = Game::width/ 2;
			y = Game::height / 2;
			dx = dy = 0;
			if (--livesCount < 0) {
				active = false;
				if (Game::saucer) Game::saucer->destroy();
				Game::gameState = (Game::score > HighScores::lowestHighScore) ? GameState::GAMEOVER : GameState::MENU;
			}
		}
	}

}

void Ship::drawLives(sf::RenderWindow & window) {
	for (int i = 0; i < livesCount; i++) {
		lives.setPosition(1000 + 40 * i, 60);
		window.draw(lives);
	}
}
