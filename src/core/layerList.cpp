#include "layerList.hpp"

#include "drawingBoard.hpp"

#include "UI/log.hpp"

#include "UI/input.hpp"

#include "constants.hpp"

#include "utilities/display.hpp"
#include "graphics/immediateDrawing.hpp"

LayerList::LayerList()
	: m_hoveredLayer(nullptr), m_mousePosRelToHoveredLayer(OUTSIDE), usedCursor(nullptr)
{
}

LayerList::~LayerList() {

}

void LayerList::update() {
	computeHoveredLayerAndMouseRelPos();
	selectedLayers.checkDragging();
}

void LayerList::show(glm::mat4x4 viewMatrix, glm::mat4x4 projMatrix) {
	for (int k = 0; k < layers.size(); ++k) {
		if (layers[k]->isVisible())
			layers[k]->show(viewMatrix, projMatrix);
	}
}

void LayerList::showInDrawingBoardSpace() {
	if (mustShowAltOrigin()) {
		selectedLayers.showAltOrigin();
	}
	for (int k = 0; k < layers.size(); ++k) {
		if(layers[k]->isVisible())
			layers[k]->showInDrawingBoardSpace();
	}
}

/*void LayerList::showFrames() {
	//for (int k = 0; k < layers.size(); ++k) {
	//	layers[k]->showFrame();
	//}
	
	if (getActivLayer()->isVisible()) {
		getActivLayer()->showFrame();

		if (mustShowAltOrigin()) {
			getActivLayer()->m_transform.showAltOrigin();
		}
	}

}*/

void LayerList::addLayer(std::string imgFilePath) {
	layers.push_back(new Layer(imgFilePath, imgFilePath));
	setActivLayer(layers.size() - 1);
}

Layer* LayerList::getLayer(int index) {
	return layers[index];
}
/*Layer* LayerList::getActivLayer() {
	return selectedLayers.;
}*/
void LayerList::setActivLayer(int layerIndex) {
	selectedLayers.removeAllLayers();
	selectedLayers.addLayer(layers[layerIndex]);
}

void LayerList::computeHoveredLayerAndMouseRelPos() {
	m_hoveredLayer = nullptr;
	for (int k = layers.size()-1; k >= 0; --k) {
		if (layers[k]->isVisible()) {
			m_mousePosRelToHoveredLayer = layers[k]->m_transform.getMouseRelativePosition();
			if (m_mousePosRelToHoveredLayer != OUTSIDE) {
				m_hoveredLayer = layers[k];
				break;
			}
		}
	}
}

bool LayerList::mustShowAltOrigin() {
	return Input::keyIsDown(ALT);//|| getActivLayer()->m_transform.isDraggingRotation();
}
bool LayerList::mouseIsHoveringAltOrigin() {
	return false;//mustShowAltOrigin() && glm::length(m_activLayer->m_transform.getAltOriginInWindowSpace() - Input::getMousePosition()) < ALT_ORIGIN_RADIUS;
}
bool LayerList::canDragRotation() {
	//float distToAltCenter = glm::length(m_activLayer->m_transform.getAltOriginInWindowSpace() - Input::getMousePosition());
	return false;// mustShowAltOrigin() && START_ROTATING_MIN_RADIUS < distToAltCenter&& distToAltCenter < START_ROTATING_MAX_RADIUS;
}

void LayerList::onDoubleLeftClic() {
	if (mouseIsHoveringAltOrigin()) {
		//m_activLayer->m_transform.setAltOrigin(glm::vec2(0.0f));
	}
}

void LayerList::onLeftClicDown() {
	if (mouseIsHoveringAltOrigin()) {
		//m_activLayer->m_transform.startDraggingAltOrigin();
	}
	else if (canDragRotation()) {
		//m_activLayer->m_transform.startDraggingRotation();
	}
	else if (m_hoveredLayer)
	{
		//Change selected layers
		if (Input::keyIsDown(CTRL)) {
			if( selectedLayers.contains(m_hoveredLayer))
				selectedLayers.removeLayer(m_hoveredLayer);
			else
				selectedLayers.addLayer(m_hoveredLayer);
		}
		else {
			if (!selectedLayers.contains(m_hoveredLayer)) {
				selectedLayers.removeAllLayers();
				selectedLayers.addLayer(m_hoveredLayer);
			}
		}
		switch (m_mousePosRelToHoveredLayer)
		{
		case OUTSIDE:
			spdlog::error("[LayerList::onLeftClicDown] shoudn't have entered the switch if we're outside any layer");
			break;
		//Drag translation if inside
		case INSIDE:
			selectedLayers.startDraggingTranslation();
			break;
		//Scale towards opposite border
		case RIGHT:
			selectedLayers.startDraggingScale(m_hoveredLayer->m_transform.getMatrix() * glm::vec4(-0.5f * m_hoveredLayer->m_transform.getAspectRatio(), 0.0f, 0.0f, 1.0f));
			break;
		case TOP_RIGHT:
			selectedLayers.startDraggingScale(m_hoveredLayer->m_transform.getMatrix() * glm::vec4(-0.5f * m_hoveredLayer->m_transform.getAspectRatio(), -0.5f, 0.0f, 1.0f));
			break;
		case TOP:
			selectedLayers.startDraggingScale(m_hoveredLayer->m_transform.getMatrix() * glm::vec4(0.0f * m_hoveredLayer->m_transform.getAspectRatio(), -0.5f, 0.0f, 1.0f));
			break;
		case TOP_LEFT:
			selectedLayers.startDraggingScale(m_hoveredLayer->m_transform.getMatrix() * glm::vec4(0.5f * m_hoveredLayer->m_transform.getAspectRatio(), -0.5f, 0.0f, 1.0f));
			break;
		case LEFT:
			selectedLayers.startDraggingScale(m_hoveredLayer->m_transform.getMatrix() * glm::vec4(0.5f * m_hoveredLayer->m_transform.getAspectRatio(), 0.0f, 0.0f, 1.0f));
			break;
		case BOT_LEFT:
			selectedLayers.startDraggingScale(m_hoveredLayer->m_transform.getMatrix() * glm::vec4(0.5f * m_hoveredLayer->m_transform.getAspectRatio(), 0.5f, 0.0f, 1.0f));
			break;
		case BOT:
			selectedLayers.startDraggingScale(m_hoveredLayer->m_transform.getMatrix() * glm::vec4(0.0f * m_hoveredLayer->m_transform.getAspectRatio(), 0.5f, 0.0f, 1.0f));
			break;
		case BOT_RIGHT:
			selectedLayers.startDraggingScale(m_hoveredLayer->m_transform.getMatrix() * glm::vec4(-0.5f * m_hoveredLayer->m_transform.getAspectRatio(), 0.5f, 0.0f, 1.0f));
			break;
		default:
			spdlog::error("[LayerList::onLeftClicDown] reached default case");
			break;
		}
		//Scale towards center if ALT down
		if (Input::keyIsDown(ALT)) {
			m_hoveredLayer->m_transform.changeToAltDraggingScaleOrigin();
		}
	}
	else {
		selectedLayers.removeAllLayers();
	}
}

void LayerList::onLeftClicUp() {
	selectedLayers.endDragging();
	usedCursor = nullptr;
}

void LayerList::setCursor() {
	if (isHandlingAnInput() && usedCursor) {
		Cursor::set(usedCursor);
	}
	else {
		if (mouseIsHoveringAltOrigin()) {
			usedCursor = Cursor::fourDirections;
			Cursor::set(Cursor::fourDirections);
		}
		else if (canDragRotation()) {
			usedCursor = Cursor::rotate;
			Cursor::set(Cursor::rotate);
		}
		else {
			switch (m_mousePosRelToHoveredLayer)
			{
			case OUTSIDE:
				usedCursor = nullptr;
				Cursor::set(Cursor::standard);
				break;
			case INSIDE:
				usedCursor = Cursor::hand;
				Cursor::set(Cursor::hand);
				break;
			case RIGHT:
				usedCursor = Cursor::leftRight;
				Cursor::set(Cursor::leftRight);
				break;
			case TOP_RIGHT:
				usedCursor = Cursor::antiDiagonal;
				Cursor::set(Cursor::antiDiagonal);
				break;
			case TOP:
				usedCursor = Cursor::topBot;
				Cursor::set(Cursor::topBot);
				break;
			case TOP_LEFT:
				usedCursor = Cursor::diagonal;
				Cursor::set(Cursor::diagonal);
				break;
			case LEFT:
				usedCursor = Cursor::leftRight;
				Cursor::set(Cursor::leftRight);
				break;
			case BOT_LEFT:
				usedCursor = Cursor::antiDiagonal;
				Cursor::set(Cursor::antiDiagonal);
				break;
			case BOT:
				usedCursor = Cursor::topBot;
				Cursor::set(Cursor::topBot);
				break;
			case BOT_RIGHT:
				usedCursor = Cursor::diagonal;
				Cursor::set(Cursor::diagonal);
				break;
			default:
				spdlog::error("[LayerList::SetCursor] reached default case");
				break;
			}
		}
	}
}