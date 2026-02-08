#include <bird.h>
#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
#include<raygui.h>

#define WIN_TITLE "flappy-boid"

// Game state
typedef enum {
	STATE_MENU,
	STATE_PLAYING,
	STATE_GAMEOVER,
	STATE_EXIT,
} GameState_t;
GameState_t gameState = STATE_MENU;

// Function prototypes
static void drawMainMenu(void);

// Main
int main(void) {
	// Main window
	SetConfigFlags(FLAG_FULLSCREEN_MODE);
	InitWindow(0, 0, WIN_TITLE);
	SetTargetFPS(60);

	// Main camera
	Camera2D cam = {
		.target = (Vector2){0.0f, 0.0f},
		.offset = (Vector2){0.0f, 0.0f},
		.rotation = 0.0f,
		.zoom = 1.0f,
	};

	// Bird/Player
	Bird bird = {
		.sprite = LoadTexture("assets/sprites/bird.png"),
		.position = (Vector2){45.0f, (GetScreenHeight() / 2.0f)},
		.size = 0.1f,
		.gravity = 600,
		.jumpVel = 320,
	};

	bird.src = (Rectangle){0,0,bird.sprite.width, bird.sprite.height};
	bird.rec = (Rectangle){0,0,bird.sprite.width * bird.size, bird.sprite.height * bird.size};

	Rectangle box = {
		.x = GetScreenWidth() / 2.0f,
		.y = GetScreenHeight() - 200,
		.width = GetScreenWidth(),
		.height = 100,
	};

	// Main loop
	while (!WindowShouldClose() && gameState != STATE_EXIT) {
		switch(gameState) {
			// Main menu
			case STATE_MENU:
				BeginDrawing();
					ClearBackground(GRAY);
					drawMainMenu();
				EndDrawing();
				break;

			// Game over
			case STATE_GAMEOVER:
				BeginDrawing();
					ClearBackground(RED);
					Vector2 size = MeasureTextEx(GetFontDefault(), "GAME OVER.", 40, 1);
					DrawText("GAME OVER.", (GetScreenWidth() - size.x) / 2, (GetScreenHeight() - size.y) / 2, 40, WHITE);
				EndDrawing();
				break;

			// Draw the game and all
			case STATE_PLAYING:
				birdUpdate(&bird);
				BeginDrawing();
					ClearBackground(RAYWHITE);
					DrawFPS(10, 10);
					if(CheckCollisionRecs(bird.rec, box)) gameState = STATE_GAMEOVER;
					BeginMode2D(cam);
						// Draw bird/player and box
						DrawRectangleRec(box, RED);
						DrawTexturePro(bird.sprite, bird.src, bird.rec, (Vector2) {0,0}, 0, WHITE);
					EndMode2D();
				EndDrawing();
				break;
			case STATE_EXIT:
				break;
		}
	}

	// Terminate program
	UnloadTexture(bird.sprite);
	CloseWindow();
	return 0;
}

// Main menu
static void drawMainMenu(void) {
	// Define buttons
	const char *buttonLabels[] = { "Play", "Quit" };
	const int numButtons = sizeof(buttonLabels) / sizeof(char*);

	// Button size and stuff
	const int btnWidth = 300, btnHeight = 30;
	const int btnGap = 5;

	// Calculate button position
	const int btnStartX = (GetScreenWidth() - btnWidth) / 2;
	const int btnStartY = (GetScreenHeight() - (btnHeight * numButtons)) / 2;

	// Create buttons
	for (int i = 0; i < numButtons; i++) {
        Rectangle btnRec = { btnStartX, btnStartY + i * (btnHeight + btnGap), btnWidth, btnHeight };
        if (GuiButton(btnRec, buttonLabels[i])) {
            switch (i) {
                case 0:
					gameState = STATE_PLAYING;
					break;
                case 1:
					gameState = STATE_EXIT;
					break;
            }
        }
    }
}