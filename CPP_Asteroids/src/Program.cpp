#include "Game.h"

// Inspired by the video "Let's make 16 games in C++: Asteroids" by FamTrinli
// https://www.youtube.com/watch?v=rWaSo2usU4A
//
int main() {
    srand((unsigned int)time(0)); // seed random number generator

    Game game;
    game.run();

    return EXIT_SUCCESS;
}
