#include "layerList.hpp"

#include "drawingBoard.hpp"

#include "UI/log.hpp"

#include "UI/input.hpp"

#include "UI/settings.hpp"

#include "helper/display.hpp"
#include "graphics/immediateDrawing.hpp"

#include "layer/loadedImageLayer.hpp"
#include "layer/copyLayer.hpp"
#include "layer/shaderLayer.hpp"

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

void LayerList::showSelectedLayerGUI() {
	if (selectedLayers.size() == 1)
		selectedLayers[0]->showGUI();
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

void LayerList::createLoadedImageLayer(const std::string& imgFilePath) {
	Layer* layer = new LoadedImageLayer(imgFilePath);
	layers.addLayer(layer);
}

void LayerList::createCopyLayer(Layer* childLayer) {
	Layer* layer = new CopyLayer(childLayer);
	layers.addLayer(layer);
}

void LayerList::createShaderLayer(unsigned int previewWidth, unsigned int previewHeight, const std::string& fragmentShaderFilepath) {
	// TODO set preview size properly, and recompute the image at full size when saving !
	Layer* layer = new ShaderLayer(previewWidth, previewHeight, fragmentShaderFilepath);
	if (layer->createdSuccessfully())
		layers.addLayer(layer);
	else
		delete layer;
}

Layer* LayerList::getLayer(int index) {
	return layers[index];
}
/*Layer* LayerList::getActivLayer() {
	return selectedLayers.;
}*/
void LayerList::setSelectedLayer(Layer* layer) {
	selectedLayers.removeAllLayers();
	selectedLayers.addLayer(layer);
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
	return Input::keyIsDown(ALT) && !selectedLayers.isEmpty();//|| getActivLayer()->m_transform.isDraggingRotation();
}
bool LayerList::mouseIsHoveringAltOrigin() {
	return mustShowAltOrigin() && glm::length(selectedLayers.getAltOriginInWindowSpace() - Input::getMousePosition()) < Settings::ALT_ORIGIN_RADIUS;
}
bool LayerList::canDragRotation() {
	if (mustShowAltOrigin()) {
		float distToAltOrigin = glm::length(selectedLayers.getAltOriginInWindowSpace() - Input::getMousePosition());
		return Settings::START_ROTATING_MIN_RADIUS < distToAltOrigin && distToAltOrigin < Settings::START_ROTATING_MAX_RADIUS;
	}
	else {
		return false;
	}
}

void LayerList::onKeyDown(Key key) {
	if (auto specialKey = std::get_if<SpecialKey>(&key)) { //Check if it's a special key
		switch (*specialKey)
		{
		case ALT:
			selectedLayers.changeDraggingCenterToAltOrigin();
			break;
		case CTRL:
			break;
		case SHIFT:
			break;
		case SPACE:
			break;
		default:
			break;
		}
	}
	else if (auto c = std::get_if<char>(&key)) { //It's a char
		if (Input::keyIsDown(CTRL)) {
			switch (*c) {
			case 'd':
				for (int k = 0; k < selectedLayers.size(); ++k) {
					createCopyLayer(selectedLayers[k]);
				}
			default:
				break;
			}
		}
	}
}

void LayerList::onKeyUp(Key key) {
	if (auto specialKey = std::get_if<SpecialKey>(&key)) { //Check if it's a special key
		switch (*specialKey)
		{
		case ALT:
			selectedLayers.revertDraggingCenterToInitialOrigin();
			break;
		case CTRL:
			break;
		case SHIFT:
			break;
		case SPACE:
			break;
		default:
			break;
		}
	}
	else if (auto c = std::get_if<char>(&key)) { //It's a char

	}
}

void LayerList::onDoubleLeftClic() {
	if (mouseIsHoveringAltOrigin()) {
		selectedLayers.resetAltOrigin();
	}
}

void LayerList::onLeftClicDown() {
	//Drag alt origin
	if (mouseIsHoveringAltOrigin()) {
		selectedLayers.startDraggingAltOrigin();
	}
	//Rotate
	else if (canDragRotation()) {
		selectedLayers.startDraggingRotation();
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
		else if (!selectedLayers.contains(m_hoveredLayer)) {
			setSelectedLayer(m_hoveredLayer);
		}
		//Translate
		if (m_mousePosRelToHoveredLayer == INSIDE) {
			selectedLayers.startDraggingTranslation();
		}
		//Drag scale/aspectRatio
		else {
			glm::vec2 originInTransformSpace;
			switch (m_mousePosRelToHoveredLayer)
			{
			case OUTSIDE:
				spdlog::error("[LayerList::onLeftClicDown] shoudn't have entered the switch if we're outside any layer");
				break;
				//Drag translation if inside
			case INSIDE:
				spdlog::error("[LayerList::onLeftClicDown] shoudn't have entered the switch if we're inside a layer");
				break;
				//Scale towards opposite border
			case RIGHT:
				originInTransformSpace = glm::vec2(-0.5f * m_hoveredLayer->m_transform.getAspectRatio(), 0.0f);
				break;
			case TOP_RIGHT:
				originInTransformSpace = glm::vec2(-0.5f * m_hoveredLayer->m_transform.getAspectRatio(), -0.5f);
				break;
			case TOP:
				originInTransformSpace = glm::vec2(0.0f, -0.5f);
				break;
			case TOP_LEFT:
				originInTransformSpace = glm::vec2(0.5f * m_hoveredLayer->m_transform.getAspectRatio(), -0.5f);
				break;
			case LEFT:
				originInTransformSpace = glm::vec2(0.5f * m_hoveredLayer->m_transform.getAspectRatio(), 0.0f);
				break;
			case BOT_LEFT:
				originInTransformSpace = glm::vec2(0.5f * m_hoveredLayer->m_transform.getAspectRatio(), 0.5f);
				break;
			case BOT:
				originInTransformSpace = glm::vec2(0.0f, 0.5f);
				break;
			case BOT_RIGHT:
				originInTransformSpace = glm::vec2(-0.5f * m_hoveredLayer->m_transform.getAspectRatio(), 0.5f);
				break;
			default:
				spdlog::error("[LayerList::onLeftClicDown] reached default case");
				break;
			}
			// Unlock the correct u and/or v aspect ratio in case we switch for dragging ratio
			bool unlockUForClickedLayer = (originInTransformSpace.x != 0.0f);
			bool unlockVForClickedLayer = (originInTransformSpace.y != 0.0f);
			// Start dragging
			selectedLayers.startDraggingScale(m_hoveredLayer, m_hoveredLayer->m_transform.getMatrix() * glm::vec4(originInTransformSpace, 0.0f, 1.0f), m_hoveredLayer->m_transform.getUAxis(), m_hoveredLayer->m_transform.getVAxis(), unlockUForClickedLayer, unlockVForClickedLayer);
			//Scale towards alt origin if ALT down
			if (Input::keyIsDown(ALT)) {
				selectedLayers.changeDraggingCenterToAltOrigin();
			}
		}
	}
	else {
		selectedLayers.removeAllLayers();
	}
}

void LayerList::onLeftClicUp() {
	if(m_hoveredLayer)
		selectedLayers.pushStateInHistoryAtTheEndOfDragging();
	selectedLayers.endDragging();
	usedCursor = nullptr;
}

void LayerList::onScroll(float motion) {
	if (motion < 0.0f) {
		selectedLayers.scale(Settings::ZOOM_FACTOR, true);
	}
	else {
		selectedLayers.scale(1.0f / Settings::ZOOM_FACTOR, true);
	}
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