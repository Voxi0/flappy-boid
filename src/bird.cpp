#include <bird.hpp>

void Bird::init(std::string_view textureFilePath, float xpos, float scale) {
	this->texture = LoadTexture(textureFilePath.data());
	this->srcRect = {
		.x = 0,
		.y = 0,
		.width = static_cast<float>(this->texture.width),
		.height = static_cast<float>(this->texture.height),
	};
	this->dstRect = {
		.x = xpos,
		.y = static_cast<float>(GetScreenHeight() / 2.0f),
		.width = this->srcRect.width * scale,
		.height = this->srcRect.height * scale,
	};
}

void Bird::update(int gravity) {
	// Gravity
	this->velocity.y += gravity * GetFrameTime();

	// Jump/Flap
	if(IsKeyPressed(KEY_SPACE)) this->velocity.y = this->jumpForce;

	// Apply velocity
	this->dstRect.y += this->velocity.y * GetFrameTime();
}

void Bird::render(void) {
	// Rotate bird towards the direction it's going (Up or down)
	float targetRotation = this->velocity.y * 0.1;
	this->rotation = Lerp(this->rotation, targetRotation, GetFrameTime() * 10.0f);
	this->rotation = Clamp(this->rotation, -35.0f, 30.0f);

	// We want to rotate the bird from it's center or else it'd look really weird
	Vector2 originPoint {this->dstRect.width / 2, this->dstRect.height / 2};

	// Draw the bird
	DrawTexturePro(
		this->texture,
		this->srcRect,
		{
			.x = this->dstRect.x + originPoint.x,
			.y = this->dstRect.y + originPoint.y,
			.width = this->dstRect.width,
			.height = this->dstRect.height,
		},
		originPoint,
		this->rotation,
		WHITE
	);
}

Bird::~Bird(void) {
	UnloadTexture(this->texture);
}