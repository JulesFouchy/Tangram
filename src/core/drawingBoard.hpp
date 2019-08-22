#pragma once

class DrawingBoard {
public:
	DrawingBoard(float whRatio);
	~DrawingBoard();
	void show();

private:
	float m_whRatio;
	void showFrame();
};