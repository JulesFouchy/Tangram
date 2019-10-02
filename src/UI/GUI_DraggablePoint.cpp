#include "GUI_DraggablePoint.hpp"

bool TanGUI::DragPoint(DraggablePoint& myPoint) {
	myPoint.show();
	return myPoint.checkDragging();
}