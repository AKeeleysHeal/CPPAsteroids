#include "Game.h"

int HighScores::lowestHighScore;

void HighScores::read() {
	lowestHighScore = 0;
	sf::Http http("localhost:3000");
	sf::Http::Request request("scores.aspx?apiKey=42&action=read");
	sf::Http::Response response = http.sendRequest(request, sf::milliseconds(2000));
	if (response.getStatus() == sf::Http::Response::Ok) {
		parse(response.getBody());
	}
	else {
		std::cout << "WARNING: Unable to connect to highscores today. Sorry for the inconvenience.\n";
	}
}

void HighScores::parse(const std::string& scoreData) {
    unsigned int prev = 0, pos = 0, len = scoreData.length();
    std::ostringstream stream;
    stream << " - HIGH SCORES -\n\n";
    while (pos < len && prev < len) {
        pos = scoreData.find(';', prev);
        if (pos == std::string::npos) pos = len;
        std::string token = scoreData.substr(prev, pos - prev);
        if (!token.empty()) {
            unsigned int pos = token.find(",");
            stream << std::setw(10) << std::left << token.substr(0, pos) << ' '
                << std::setw(6) << std::right << token.substr(pos + 1) << '\n';
            lowestHighScore = atoi(token.substr(pos + 1).c_str());
        }
        prev = pos + 1;
    }
	Game::text["highscores"].setString(stream.str());
}

void HighScores::write(const std::string& name, const std::string& score) {
	std::string url = "scores.aspx?apiKey=42&action=write"
		"&name=" + name +
		"&score=" + score;
	sf::Http http("localhost:3000");
	sf::Http::Request request(url);
	sf::Http::Response response = http.sendRequest(request, sf::milliseconds(500));
	if (response.getStatus() != sf::Http::Response::Ok) {
		std::cout << "WARNING: Unable to connect to highscores today. Sorry for the inconvenience.\n";
	}
	read();
}
