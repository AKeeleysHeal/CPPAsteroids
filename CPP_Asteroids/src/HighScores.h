#pragma once

class HighScores {
public:
    static int lowestHighScore;
    static void read();
    static void write(const std::string& name, const std::string& score);

private:
    static void parse(const std::string& response);
};
