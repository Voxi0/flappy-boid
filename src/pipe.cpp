#include <pipe.hpp>

void Pipe::spawn(float x, float gapY, float gapHeight, bool isTopPipe) {
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

void Pipe::update(void) {
	this->dstRect.x -= this->moveSpeed * GetFrameTime();
}

void Pipe::render(Texture2D &texture) {
	DrawTexturePro(texture, this->srcRect, this->dstRect, {0,0}, 0.0f, WHITE);
}