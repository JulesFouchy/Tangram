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
		ImGui::BeginChild(layer->getName().c_str(), ImVec2(ImGui::GetWindowContentRegionWidth() * 0.95f, 150), true, ImGuiWindowFlags_NoMove);
		ImGui::PushID((int)layer);
		dragDropSourceReorderLayer(layerID);
		// onClic : add/remove layer from selection
		bool b = ImGui::Selectable(layer->getName().c_str(), layerList.selectedLayers.contains(layerID));
		dragDropSourceReorderLayer(layerID);
		if (b) {
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
		// miniature
		ImGui::Image((ImTextureID)layer->getTexture().getID(), ImVec2(50 * layer->getTexture().getAspectRatio(), 50), ImVec2(0, 1), ImVec2(1, 0), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
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
		ImGui::Image((ImTextureID)layer->getTexture().getID(), ImVec2(50 * layer->getTexture().getAspectRatio(), 50), ImVec2(0, 1), ImVec2(1, 0), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
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