#include<raylib.h>
#include<math.h>
#include<bird.h>

void birdUpdate(Bird *bird) {
	float deltaTime = GetFrameTime();

	// Gravity
	bird->velocity.y += bird->gravity * deltaTime;
	
	// Movement
	if(IsKeyPressed(KEY_SPACE)) bird->velocity.y = -bird->jumpVel;
	if(IsKeyDown(KEY_D)) bird->velocity.x += bird->gravity * deltaTime;
	else if(IsKeyDown(KEY_A)) bird->velocity.x -= bird->gravity * deltaTime;

	// Friction
	bird->velocity.x *= powf(0.1f, deltaTime);

	// Apply velocity
	bird->dst.x += bird->velocity.x * deltaTime;
	bird->dst.y += bird->velocity.y * deltaTime;
}