#include <string>
#include <raylib.h>
#include <raymath.h>

#define WIN_TITLE "test"
#define NUM_PIPES 16
Texture2D pipeTexture;

struct Bird {
	Texture2D texture {};
	Rectangle srcRect {}, dstRect {};
	Vector2 velocity {};
	float gravity {980.0f}, jumpForce {-350.0f}, rotation {};

	void init(std::string_view textureFilePath) {
		this->texture = LoadTexture(textureFilePath.data());
	}

	void update(void) {
		// Gravity
		this->velocity.y += this->gravity * GetFrameTime();

		// Jump/Flap
		if(IsKeyPressed(KEY_SPACE)) this->velocity.y = this->jumpForce;

		// Apply velocity
		this->dstRect.y += this->velocity.y * GetFrameTime();
	}

	void render(void) {
		float targetRotation = this->velocity.y * 0.1;
		this->rotation = Lerp(this->rotation, targetRotation, GetFrameTime() * 10.0f);
		this->rotation = Clamp(this->rotation, -35.0f, 30.0f);
		DrawTexturePro(this->texture, this->srcRect, this->dstRect, {this->dstRect.width / 2, this->dstRect.height / 2}, this->rotation, WHITE);
	}

	~Bird(void) {
		UnloadTexture(this->texture);
	}
};

struct Pipe {
	Rectangle srcRect {}, dstRect {};
	int moveSpeed {90}, pipeGap {220};
	bool passed {false};

	void spawn(float x, float gapY, float gapHeight, bool isTopPipe) {
		if(isTopPipe) {
			float height = gapY - (gapHeight / 2.0f);
			this->dstRect = {x, 0, this->dstRect.width, height};
			this->srcRect.height *= 1;
		} else {
			float gapBottom {gapY + (gapHeight / 2.0f)};
			float height {static_cast<float>(GetScreenHeight()) - gapBottom};
			this->dstRect = {x, gapBottom, this->dstRect.width, height};
			this->srcRect.height *= -1;
		}
	}

	void update(void) {
		this->dstRect.x -= this->moveSpeed * GetFrameTime();
	}

	void render(void) {
		DrawTexturePro(pipeTexture, this->srcRect, this->dstRect, {0,0}, 0.0f, WHITE);
	}
};

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
		Game(int winWidth, int winHeight, std::string_view winTitle, unsigned int cfgFlags) {
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
		void run(void) {
			while(!WindowShouldClose()) {
				this->update();
				this->render();
			}
		}

		// Deconstructor
		~Game(void) {
			UnloadTexture(pipeTexture);
			CloseWindow();
		}
	private:
		GameState gameState {STATE_PLAYING};
		Bird bird;
		Pipe pipes[NUM_PIPES];
		float spawnTimer { 0.0f };

		void spawnPipePair(void) {
			int paired {};
			for(Pipe &pipe : this->pipes) {
				if (pipe.dstRect.x < -pipe.dstRect.width || pipe.dstRect.x == 0) {
					pipe.spawn(GetScreenWidth() + 50, GetRandomValue(150, GetScreenHeight() - 150), 220, (paired == 0));
					paired++;
					if(paired == 2) break;
				}
			}
		}
		void update(void) {
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
		void render(void) {
			BeginDrawing();
				ClearBackground(BLACK);
				DrawFPS(10, 10);
				this->bird.render();
				for(Pipe &pipe : this->pipes) pipe.render();
			EndDrawing();
		}
};

int main(void) {
	Game game(0, 0, WIN_TITLE, FLAG_FULLSCREEN_MODE | FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
	game.run();
	return 0;
}