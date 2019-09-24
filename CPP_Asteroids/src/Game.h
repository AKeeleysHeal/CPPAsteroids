#pragma once
#include "Program.h"
#include "HighScores.h"
#include "Animation.h"
#include "Entity.h"
#include "Ship.h"
#include "Saucer.h"
#include "Laser.h"
#include "Asteroid.h"
#include "Explosion.h"

enum GameState { MENU, PLAYING, GAMEOVER };

class Game {
private:
	sf::RenderWindow window;
	sf::Texture outerSpace, spriteSheet;
	sf::Sprite background;
	sf::Music music;
	sf::Font font;
	sf::Clock gameTime;
	std::string playerName;
public:
	static const int height = 800;
	static const int width = 1200;
	static GameState gameState;
	static std::unordered_map<std::string, sf::Sound> sounds;
	static std::unordered_map<std::string, Animation> animations;
	static std::unordered_map<std::string, sf::Text> text;
	static std::list<Entity*> entities, curEntities;
	static Ship *ship;
	static Saucer *saucer;
	static int score;
public:
	Game();
	~Game();
	void run();
private:
	void init();
	void loadContent();
	sf::SoundBuffer& loadSoundFile(const std::string& fileName);
	void pollEvents();
	void handleKeyPress();
	void update();
	void draw();
};

// a little global helper function to return a random number as a float
float randf(int n);
