#pragma once

#include <vector>
#include "action.hpp"

class History {
public:
	History();
	~History();

	void beginUndoGroup();
	void endUndoGroup();
	void addAction(Action action);
	void moveBackward();
	void moveForward();

public:
	int getCurrentDelimiter();
private:
	std::vector<Action> m_actions;
	std::vector<int> m_undoGroupDelimiters;
	int m_currentDelimiterIndex;
};