#pragma once

#include<raylib.h>

typedef struct {
	Rectangle src, dst;
	int moveSpeed;
} Pipe;

void pipeUpdate(Pipe *pipe);