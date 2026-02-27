#include <pipe.hpp>

void Pipe::spawn(float x, float gap, float gapTopY, bool isTopPipe) {
	float screenHeight {static_cast<float>(GetScreenHeight())};

	// Top pipe
	if(isTopPipe) {
		this->dstRect = {
			.x = x,
			.y = 0,
			.width = this->dstRect.width,
			.height = gapTopY,
		};
	}

	// Bottom pipe
	else {
		float bottomPipeY {gapTopY + gap};
		this->dstRect = {
			.x = x,
			.y = bottomPipeY,
			.width = this->dstRect.width,
			.height = screenHeight - bottomPipeY,
		};
	}
}

void Pipe::update(void) {
	this->dstRect.x -= this->moveSpeed * GetFrameTime();
}

void Pipe::render(Texture2D &base, Texture2D &cap, Rectangle &baseSrc, Rectangle &capSrc) {
	// Pipe base
	DrawTexturePro(
		base,
		baseSrc,
		this->dstRect,
		{0,0},
		0.0f,
		WHITE
	);

	// Pipe cap
	DrawTexturePro(
		cap,
		{
			capSrc.x, capSrc.y, capSrc.width, this->srcRect.height,
		},
		{
			.x = this->dstRect.x,
			.y = this->dstRect.y,
			.width = this->dstRect.width,
			.height = capSrc.height,
		},
		{0,0},
		0.0f,
		WHITE
	);
}