#pragma once

#include <string>
#include <raylib.h>
#include <bird.hpp>
#include <pipe.hpp>

#define NUM_PIPES 16

typedef enum {
	STATE_PLAYING,
	STATE_GAMEOVER,
} GameState;

class Game {
	public:
		// Disallow copies of the Game object
		Game(const Game&) = delete;
		Game& operator=(const Game&) = delete;

		// Constructor - Initialize everything
		Game(int winWidth, int winHeight, std::string_view winTitle, unsigned int cfgFlags);

		// Main loop
		void run(void);

		// Deconstructor
		~Game(void);
	private:
		GameState gameState {STATE_PLAYING};
		Bird bird;
		Pipe pipes[NUM_PIPES];
		float spawnTimer { 0.0f };

		void spawnPipePair(void);
		void update(void);
		void render(void);
};