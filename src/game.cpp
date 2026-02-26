#include <game.hpp>

// We load one texture image for both top and bottom pipes
// We can easily flip the image why bother storing two textures?
Texture2D pipeTexture;

// Spawn a pair of pipes
void Game::spawnPipePair(void) {
	int paired {};
	for(Pipe &pipe : this->pipes) {
		if (pipe.dstRect.x < -pipe.dstRect.width || pipe.dstRect.x == 0) {
			pipe.spawn(GetScreenWidth() + 50, GetRandomValue(150, GetScreenHeight() - 150), 220, (paired == 0));
			paired++;
			if(paired == 2) break;
		}
	}
}

// Initialize everything
Game::Game(int winWidth, int winHeight, std::string_view winTitle, unsigned int cfgFlags) {
	// Main window
	SetConfigFlags(cfgFlags);
	InitWindow(winWidth, winHeight, winTitle.data());

	// Load pipe texture
	pipeTexture = LoadTexture("assets/sprites/pipe.png");

	// Create bird/player
	this->bird.init("assets/sprites/bird.png");
	this->bird.srcRect = {
		.x = 0,
		.y = 0,
		.width = static_cast<float>(this->bird.texture.width),
		.height = static_cast<float>(this->bird.texture.height),
	};
	this->bird.dstRect = {
		.x = 250.0f,
		.y = static_cast<float>(GetScreenHeight() / 2.0f),
		.width = this->bird.srcRect.width * 0.1f,
		.height = this->bird.texture.height * 0.1f,
	};

	// Pipes
	for(Pipe &pipe : this->pipes) {
		pipe.srcRect = {0, 0, static_cast<float>(pipeTexture.width), static_cast<float>(pipeTexture.height)};
		pipe.dstRect.width = 50;
	}
}

// Main loop
void Game::run(void) {
	while(!WindowShouldClose()) {
		this->update();
		this->render();
	}
}

// Update everything
void Game::update(void) {
	this->bird.update();

	this->spawnTimer += GetFrameTime();
	if(spawnTimer >= 1.5f) {
		this->spawnPipePair();
		this->spawnTimer = 0;
	}

	for(Pipe &pipe : this->pipes) {
		pipe.update();
		if(CheckCollisionRecs(this->bird.dstRect, pipe.dstRect)) this->gameState = STATE_GAMEOVER;
	}
}

// Render everything
void Game::render(void) {
	BeginDrawing();
		ClearBackground(BLACK);
		DrawFPS(10, 10);
		this->bird.render();
		for(Pipe &pipe : this->pipes) pipe.render(pipeTexture);
	EndDrawing();
}

// De-initialize everything
Game::~Game(void) {
	UnloadTexture(pipeTexture);
	CloseWindow();
}