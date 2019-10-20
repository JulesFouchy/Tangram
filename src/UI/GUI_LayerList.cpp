#include "GUI_LayerList.hpp"

#include "imgui/imgui.h"

#include "UI/input.hpp"
#include "UI/log.hpp"

#include "core/drawingBoard.hpp"

void GUI_LayerList::show() {

	LayerList& layerList = DrawingBoard::LayerRegistry().AliveLayers();

	ImGui::Begin("Layers");
	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.95f);

	ImGui::Text("-------------------------"); 
	dragDropTargetReorderLayer(layerList.layers.size());

	for (int k = layerList.layers.size() - 1; k >= 0; --k) {
		LayerID layerID = layerList.layers[k];
		Layer* layer = DrawingBoard::LayerRegistry()[layerID];
		//
		if (layerList.selectedLayers.contains(layerID))
			ImGuiBeginLayerChildWindow(layerID, 150);
		else {
			// miniature
			ImGuiShowTexture(layer, 30);
			ImGui::SameLine();
			ImGuiBeginLayerChildWindow(layerID, 25);
		}
		ImGui::PushID(layerID);
		dragDropSourceReorderLayer(layerID);
		// onClic : add/remove layer from selection
		bool bClicToSelect = ImGui::Selectable(layer->getName().c_str(), layerList.selectedLayers.contains(layerID));
		dragDropSourceReorderLayer(layerID);
		if (bClicToSelect) {
			if (Input::keyIsDown(CTRL)) {
				if (layerList.selectedLayers.contains(layerID))
					layerList.selectedLayers.removeLayerByIndexInGroup(layerID);
				else
					layerList.selectedLayers.addLayer(layerID);
			}
			else {
				layerList.setSelectedLayer(layerID);
			}
		}
		if (layerList.selectedLayers.contains(layerID)) {
			// miniature
			ImGuiShowTexture(layer, 50);
			// visibiliy checkbox
			ImGui::Checkbox("Visible", layer->getIsVisiblePointer());
			// movability checkbox
			ImGui::Checkbox("Movable", layer->getIsMovablePointer());
			// reset transform
			if (ImGui::Button("Reset Transform")) {
				DrawingBoard::history.beginUndoGroup();
				layer->m_transform.reset(true);
				DrawingBoard::history.endUndoGroup();
			}
		}
		ImGui::PopID();
		ImGui::EndChild();
		ImGui::Text("-------------------------");
		dragDropTargetReorderLayer(k);
		//
	}
	ImGui::PopItemWidth();
	ImGui::End();

}

void GUI_LayerList::dragDropSourceReorderLayer(LayerID layerID) {
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
	{
		Layer* layer = DrawingBoard::LayerRegistry()[layerID];
		ImGui::SetDragDropPayload("Reordering layer", &layerID, sizeof(LayerID));
		ImGui::Image((ImTextureID)layer->getTexture_Preview().getID(), ImVec2(50 * layer->getTexture_Preview().getAspectRatio(), 50), ImVec2(0, 1), ImVec2(1, 0), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
		ImGui::EndDragDropSource();
	}
}

void GUI_LayerList::dragDropTargetReorderLayer(int layerIndex) {
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload * payload = ImGui::AcceptDragDropPayload("Reordering layer"))
		{
			IM_ASSERT(payload->DataSize == sizeof(LayerID));
			LayerID payload_layerID = *(LayerID *)payload->Data;
			DrawingBoard::LayerRegistry().AliveLayers().layers.reorderLayer(payload_layerID, layerIndex);
		}
		ImGui::EndDragDropTarget();
	}
}

void GUI_LayerList::ImGuiShowTexture(Layer* layer, unsigned int previewHeight) {
	ImGui::Image((ImTextureID)layer->getTexture_Preview().getID(), ImVec2(previewHeight * layer->getTexture_Preview().getAspectRatio(), previewHeight), ImVec2(0, 1), ImVec2(1, 0), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
}


void GUI_LayerList::ImGuiBeginLayerChildWindow(LayerID layerID, unsigned int height) {
	Layer* layer = DrawingBoard::LayerRegistry()[layerID];
	ImGui::BeginChild((layer->getName() + std::to_string(layerID)).c_str(), ImVec2(ImGui::GetWindowContentRegionWidth() * 0.95f, height), true, ImGuiWindowFlags_NoMove);
}