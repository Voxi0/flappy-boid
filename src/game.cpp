#include <game.hpp>

static Texture2D pipeBase, pipeCap;
static Rectangle pipeBaseSrc {
	.x = 0,
	.y = 0,
	.height = 10,
};
static Rectangle pipeCapSrc {
	.x = 0,
	.height = 10,
};

// Initialize everything
Game::Game(int winWidth, int winHeight, std::string_view winTitle, unsigned int cfgFlags) {
	// Main window
	SetConfigFlags(cfgFlags);
	InitWindow(winWidth, winHeight, winTitle.data());

	// Load pipe texture
	pipeBase = LoadTexture("assets/sprites/pipe_base.png");
	pipeCap = LoadTexture("assets/sprites/pipe_cap.png");

	pipeBaseSrc.width = static_cast<float>(pipeBase.width);
	pipeCapSrc.y = static_cast<float>(pipeCap.height - 10);
	pipeCapSrc.width = static_cast<float>(pipeCap.width);

	// Bird/player
	this->bird.init("assets/sprites/bird.png", 250.0f, 0.1f);

	// Pipes
	for(unsigned long i = 0; i < this->pipes.size(); i++) {
		this->pipes[i].moveSpeed = 250;

		// Flip the texture for bottom pipes
		if(i % 2 == 0) this->pipes[i].srcRect.height *= -1;
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
	this->bird.update(GRAVITY);

	// Spawn a new pipe
	this->spawnTimer += GetFrameTime();
	if(this->spawnTimer >= 3.5f) {
		this->spawnPipePair();
		this->spawnTimer = 0;
	}

	// Update all pipes
	for(Pipe &pipe : this->pipes) {
		pipe.update();
		if((pipe.dstRect.x + pipe.dstRect.width) < 0) pipe.active = false;
		if(CheckCollisionRecs(this->bird.dstRect, pipe.dstRect)) this->gameState = STATE_GAMEOVER;
	}
}

// Render everything
void Game::render(void) {
	BeginDrawing();
		ClearBackground(BLACK);
		DrawFPS(10, 10);
		this->bird.render();
		for(Pipe &pipe : this->pipes) {pipe.render(pipeBase, pipeCap, pipeBaseSrc, pipeCapSrc);}
		if(this->gameState == STATE_GAMEOVER) {DrawText("YOU DIED VRO", 10, 40, 40, RED);}
	EndDrawing();
}

// Spawn a pair of pipes
void Game::spawnPipePair(void) {
	for(unsigned long i = 0; i < this->pipes.size(); i++) {
		if(!this->pipes[i].active) {
			int pipeTop {GetRandomValue(MIN_PIPE_HEIGHT, GetScreenHeight() - PIPE_GAP)};

			// Top pipe
			this->pipes[i].spawn(GetScreenWidth(), PIPE_GAP, pipeTop, false);
			this->pipes[i].active = true;

			// Bottom pipe
			this->pipes[i+1].spawn(GetScreenWidth(), PIPE_GAP, pipeTop, true);
			this->pipes[i+1].active = true;

			break;
		}
	}
}

// De-initialize everything
Game::~Game(void) {
	UnloadTexture(pipeBase);
	UnloadTexture(pipeCap);
	CloseWindow();
}