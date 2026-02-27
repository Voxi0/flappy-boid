#pragma once

#include <array>
#include <string>
#include <raylib.h>
#include <bird.hpp>
#include <pipe.hpp>

// Constants
#define GRAVITY 980

// Pipes
#define NUM_PIPES 8
#define MIN_PIPE_HEIGHT -50
#define PIPE_GAP 200

typedef enum {
	STATE_PLAYING,
	STATE_GAMEOVER,
} GameState;

class Game {
	public:
		// Disallow copies of this class
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
		std::array<Pipe, NUM_PIPES> pipes;
		float spawnTimer { 0.0f };

		void spawnPipePair(void);
		void update(void);
		void render(void);
};