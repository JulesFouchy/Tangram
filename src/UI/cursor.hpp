#pragma once

#include "SDL.h"

typedef SDL_Cursor CursorStyle;

class Cursor {
public:
	static void Initialize();
	static void ShutDown();

	static void set(CursorStyle* cursorPtr);
public:
	static CursorStyle* standard;
	static CursorStyle* hand;
	static CursorStyle* leftRight;
	static CursorStyle* antiDiagonal;
	static CursorStyle* topBot;
	static CursorStyle* diagonal;
	static CursorStyle* fourDirections;
private:
	Cursor() = default;
	~Cursor() = default;
};