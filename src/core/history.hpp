#pragma once

#include <vector>
#include "undoGroup.hpp"

class History {
public:
	History();
	~History();

	void addUndoGroup(UndoGroup undo);
	void moveBack();
	void moveForward();

private:
	std::vector<UndoGroup> m_undoGroups;
	int m_currentIndex;
};