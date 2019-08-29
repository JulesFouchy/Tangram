#pragma once

#include "SDL.h"

#include <string>

typedef SDL_Cursor CursorType;

class Cursor {
public:
	static void Initialize();
	static void ShutDown();

	static void set(CursorType* cursorPtr);
public:
	static CursorType* standard;
	static CursorType* hand;
	static CursorType* leftRight;
	static CursorType* antiDiagonal;
	static CursorType* topBot;
	static CursorType* diagonal;
	static CursorType* fourDirections;
	static CursorType* wait;
	static CursorType* rotate;
private:
	static CursorType* createCustomCursor(std::string imagePath);
private:
	Cursor() = default;
	~Cursor() = default;
};