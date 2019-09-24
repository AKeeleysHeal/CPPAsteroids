#include "Game.h"
GameState Game::gameState = GameState::MENU;
std::unordered_map<std::string, sf::Sound> Game::sounds;
std::unordered_map<std::string, Animation> Game::animations;
std::unordered_map<std::string, sf::Text> Game::text;
std::list<Entity*> Game::entities, Game::curEntities;
Ship *Game::ship;
Saucer *Game::saucer;
int Game::score;

Game::Game() {
	window.create(sf::VideoMode(width, height), "asteroids");
	window.setFramerateLimit(60);
	loadContent();
	//HighScores::read(); sychronous
	std::thread(HighScores::read).detach();
}

Game::~Game() {
	for (Entity* e : entities) {
		delete e;
	}
}

void Game::run() {
	while (window.isOpen()) {
		pollEvents();
		handleKeyPress();
		update();
		draw();
	}
}

void Game::init() {
	while (!entities.empty()) {
		delete entities.front();
		entities.pop_front();
	}
	score = 0;
	playerName = "";
	saucer = nullptr;
	ship = new Ship(animations["shipStill"], width / 2, height / 2);
	entities.push_back(ship);
	for (int i = 0; i < 4; i++) entities.push_back(new Asteroid(animations["asteroidLg"], 0, randf(height)));
}

void Game::loadContent() {
	font.loadFromFile("fonts/ocr.ttf");
	outerSpace.loadFromFile("images/outerspace.jpg");
	spriteSheet.loadFromFile("images/spritesheet.jpg");

	animations["shipStill"] = new Animation(spriteSheet, 1, 42, 140, 1820, 384, 0, -35);
	animations["shipMove"] = new Animation(spriteSheet, 6, 42, 140, 1862, 384, 0.25f, -35);
	animations["laser"] = new Animation(spriteSheet, 8, 32, 48, 1540, 384, 0.33f);
	animations["asteroidSm"] = new Animation(spriteSheet, 16, 40, 40, 0, 524, 0.2f);
	animations["asteroidMd"] = new Animation(spriteSheet, 16, 60, 60, 0, 464, 0.2f);
	animations["asteroidLg"] = new Animation(spriteSheet, 16, 80, 80, 0, 384, 0.2f);
	animations["explosionAsteroid"] = new Animation(spriteSheet, 32, 192, 192, 0, 0, 0.15f);
	animations["explosionShip"] = new Animation(spriteSheet, 32, 192, 192, 0, 192, 0.15f);
	animations["flySauce"] = new Animation(spriteSheet, 6, 45, 73, 2200, 384, 0.15f);
	animations["laserSauce"] = new Animation(spriteSheet, 8, 32, 48, 1540, 444, 0.33f);

	//loading all sounds
	sounds["pewPew"].setBuffer(loadSoundFile("audio/laser.wav"));
	sounds["thrust"].setBuffer(loadSoundFile("audio/thrust.wav"));
	sounds["saucer"].setBuffer(loadSoundFile("audio/saucer.wav"));
	sounds["smallBang"].setBuffer(loadSoundFile("audio/bang_sm.wav"));
	sounds["medBang"].setBuffer(loadSoundFile("audio/bang_md.wav"));
	sounds["bigBang"].setBuffer(loadSoundFile("audio/bang_lg.wav"));

	text["highscores"] = sf::Text("loading", font, 30);
	text["highscores"].setPosition(460, 80);
	text["begin"] = sf::Text("Press any key to begin.", font, 30);
	text["begin"].setPosition(400, 550);
	text["score"] = sf::Text("Score: 0", font, 30);
	text["score"].setPosition(20, 20);
	text["congrats"] = sf::Text("You set a new higscore!", font, 30);
	text["congrats"].setPosition(420, 150);
	text["name"] = sf::Text("Name: ", font, 30);
	text["name"].setPosition(420, 220); 
	text["cursor"] = sf::Text("*", font, 30);
	text["cursor"].setFillColor(sf::Color::Cyan);

	music.openFromFile("audio/solvethepuzzle.ogg");
	music.setLoop(true);
	music.setVolume(10);
	music.play();
}

sf::SoundBuffer & Game::loadSoundFile(const std::string & fileName) {
	sf::SoundBuffer* buffer = new sf::SoundBuffer();
	buffer->loadFromFile(fileName);
	return *buffer;
}

void Game::pollEvents() {
	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window.close();
		}
		else if (gameState == GameState::MENU && event.type == sf::Event::KeyReleased) {
			init();
			gameState = GameState::PLAYING;
			std::this_thread::sleep_for(std::chrono::milliseconds(333));
			while (window.pollEvent(event)); //this does nothing; condition just calls method; empty event queue
		}
		else if (gameState == GameState::PLAYING && event.type == sf::Event::KeyReleased) {
			if (event.key.code == sf::Keyboard::Space) {
				entities.push_back(new Laser(animations["laser"], ship->x, ship->y, ship->heading));
				sounds["pewPew"].play();
			}
		}
		else if (gameState == GameState::GAMEOVER && event.type == sf::Event::TextEntered) {
			int u = event.text.unicode;
			if (u == ENTER && playerName.length() != 0) {
				gameState = GameState::MENU;
				std::this_thread::sleep_for(std::chrono::milliseconds(333));
				while (window.pollEvent(event)); //this does nothing; condition just calls method; empty event queue
				//TODO read highscores and write them
			}
			else if (u == BACKSPACE && playerName.length() != 0) {
				playerName = playerName.substr(0, playerName.length() - 1);
			}
			else if (u > SPACE && u < EXTENDED && playerName.length() < 10) {
				playerName += static_cast<char>(u);
			}
		}
	}
}

void Game::handleKeyPress() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
	if (gameState == GameState::PLAYING) {
		ship->thrusting = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) ship->heading -= 3;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) ship->heading += 3;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
			entities.push_back(new Laser(animations["laser"], ship->x, ship->y, ship->heading));
		}
	}
}

void Game::update() {
	if (gameState == GameState::PLAYING) {
		curEntities = std::list<Entity*>(entities);
		for(Entity* entity : curEntities) {
			entity->update();
			entity->collisionCheck();
			if (!entity->active) entities->remove(entity);
		}
		if (std::rand() % 900 == 0) entities.push_back(new Asteroid(animations["asteroidLg"], 0, randf(height)));

		if (std::rand() % 900 == 0 && !saucer) {
			saucer = new Saucer(animations["flySauce"], 0, (height * 0.25f) + randf(height));
			entities.push_back(saucer);
		}
		text["score"].setString("Score: " + std::to_string(score));
	}
	else if (gameState == GameState::GAMEOVER) {
		text["name"].setString("Name: " + playerName);
		if (gameTime.getElapsedTime().asMilliseconds() % 600 > 300) {
			float pos = text["name"].getGlobalBounds().left + text["name"].getGlobalBounds().width;
			text["cursor"].setPosition(pos, 220);
		}
		else {
			text["cursor"].setPosition(-100, -100);
		}
	}

}

void Game::draw() {
	window.draw(background);
	if (gameState == GameState::MENU) {
		window.draw(text["highscores"]);
		window.draw(text["begin"]);
	}	
	if (gameState == GameState::PLAYING) {
		for (Entity* entity : entities) {
			entity->draw(window);
		}
		ship->drawLives(window);
		window.draw(text["score"]);
	}
	if (gameState == GameState::GAMEOVER) {
		window.draw(text["congrats"]);
		window.draw(text["name"]);
		window.draw(text["cursor"]);
	}
	window.display();
}

// a little global helper function to return a random number as a float
float randf(int n) {
	return static_cast<float>(rand() % n); // number between 0 and n-1
}