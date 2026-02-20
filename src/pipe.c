#include<pipe.h>

void pipeUpdate(Pipe *pipe) {
	const float deltaTime = GetFrameTime();
	pipe->dst.x -= pipe->moveSpeed * deltaTime;
}