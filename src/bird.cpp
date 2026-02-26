#include <bird.hpp>

void Bird::init(std::string_view textureFilePath) {
	this->texture = LoadTexture(textureFilePath.data());
}

void Bird::update(void) {
	// Gravity
	this->velocity.y += this->gravity * GetFrameTime();

	// Jump/Flap
	if(IsKeyPressed(KEY_SPACE)) this->velocity.y = this->jumpForce;

	// Apply velocity
	this->dstRect.y += this->velocity.y * GetFrameTime();
}

void Bird::render(void) {
	float targetRotation = this->velocity.y * 0.1;
	this->rotation = Lerp(this->rotation, targetRotation, GetFrameTime() * 10.0f);
	this->rotation = Clamp(this->rotation, -35.0f, 30.0f);
	DrawTexturePro(this->texture, this->srcRect, this->dstRect, {this->dstRect.width / 2, this->dstRect.height / 2}, this->rotation, WHITE);
}

Bird::~Bird(void) {
	UnloadTexture(this->texture);
}