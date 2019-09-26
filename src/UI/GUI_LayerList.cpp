#include "GUI_LayerList.hpp"

#include "imgui/imgui.h"

#include "UI/input.hpp"
#include "UI/log.hpp"

#include "core/drawingBoard.hpp"

void GUI_LayerList::show() {

	LayerList& layerList = DrawingBoard::getLayerList();

	ImGui::Begin("Layers");
	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.95f);

	ImGui::Text("-------------------------"); 
	dragDropTargetReorderLayer(layerList.layers.size());

	for (int k = layerList.layers.size() - 1; k >= 0; --k) {
		Layer* layer = layerList.layers[k];
		//
		ImGui::BeginChild(layer->getName().c_str(), ImVec2(ImGui::GetWindowContentRegionWidth() * 0.95f, 150), true, ImGuiWindowFlags_NoMove);
		ImGui::PushID((int)layer);
		dragDropSourceReorderLayer(layer);
		// onClic : add/remove layer from selection
		bool b = ImGui::Selectable(layer->getName().c_str(), layerList.selectedLayers.contains(layer));
		dragDropSourceReorderLayer(layer);
		if (b) {
			if (Input::keyIsDown(CTRL)) {
				if (layerList.selectedLayers.contains(layer))
					layerList.selectedLayers.removeLayer(layer);
				else
					layerList.selectedLayers.addLayer(layer);
			}
			else {
				layerList.setSelectedLayer(layer);
			}
		}
		// miniature
		ImGui::Image((ImTextureID)layer->getTexture().getID(), ImVec2(50 * layer->getTexture().getAspectRatio(), 50), ImVec2(0, 1), ImVec2(1, 0), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
		// visibiliy checkbox
		ImGui::Checkbox("visible", layerList.getLayer(k)->getIsVisiblePointer());
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

void GUI_LayerList::dragDropSourceReorderLayer(Layer* layer) {
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
	{
		ImGui::SetDragDropPayload("Reordering layer", &layer, sizeof(Layer*));
		ImGui::Image((ImTextureID)layer->getTexture().getID(), ImVec2(50 * layer->getTexture().getAspectRatio(), 50), ImVec2(0, 1), ImVec2(1, 0), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
		ImGui::EndDragDropSource();
	}
}

void GUI_LayerList::dragDropTargetReorderLayer(int layerIndex) {
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload * payload = ImGui::AcceptDragDropPayload("Reordering layer"))
		{
			IM_ASSERT(payload->DataSize == sizeof(Layer*));
			Layer* payload_layer = *(Layer * *)payload->Data;
			DrawingBoard::getLayerList().layers.reorderLayer(payload_layer, layerIndex);
		}
		ImGui::EndDragDropTarget();
	}
}