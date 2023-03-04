#include "PanelResource.h"

#include "Application.h"
#include "Modules/ModuleEditor.h"
#include "Resources/Resource.h"
#include "Panels/PanelInspector.h"
#include "Utils/Logging.h"

#include "imgui.h"
#include "IconsFontAwesome5.h"

#include "Utils/Leaks.h"

PanelResource::PanelResource()
	: Panel("Welcome to ImageTools", true) {
	panelResourceType = PanelResourceType::NONE;
}

PanelResource::PanelResource(const char* _resourceName, PanelResourceType _panelResourceType)
	: Panel(_resourceName, true) {
	panelResourceType = _panelResourceType;
	resource = new Resource(this);
}

void PanelResource::DrawPanel() {
	ImGui::SetNextWindowDockID(App->editor->dockMainId, ImGuiCond_FirstUseEver);
	std::string windowName = std::string(ICON_FA_IMAGE "  ") + GetName();
	if (ImGui::Begin(windowName.c_str(), &UpdateEnabled()), ImGuiWindowFlags_AlwaysAutoResize) {
		if (ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows)) {
			App->editor->panelInspector.SetPanelResourceSelected(this);
		}

		switch (panelResourceType) {
		case PanelResourceType::NONE:
			DrawPanelDefault();
			break;
		case PanelResourceType::IMAGE:
			DrawPanelImage();
			break;
		case PanelResourceType::COMPARE_IMAGE:
			DrawPanelCompare();
			break;
		}

		ImGui::End();
	}
}

Resource* PanelResource::GetResource() const {
	return resource;
}

void PanelResource::SetResource(Resource* _resource) {
	if (_resource) {
		resource = _resource;
		hasResource = true;
	} else {
		hasResource ? true : false;
	}
}

PanelResourceType PanelResource::GetPanelResourceType() const {
	return panelResourceType;
}

void PanelResource::DrawPanelDefault() {
	ImGui::Text("Welcome to ImageTools!");
}

void PanelResource::DrawPanelImage() {
	//ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
	//if (ImGui::BeginTable("##table1", 2, ImGuiTableFlags_Resizable)) {
	//	DrawImageTable("Image 1");

	//	ImGui::EndTable();
	//}
	//ImGui::PopStyleVar();
}

void PanelResource::DrawPanelCompare() {
	//ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
	//if (ImGui::BeginTable("##table2", 2, ImGuiTableFlags_Resizable)) {
	//	DrawImageTable("Image 1");
	//	DrawImageTable("Image 2");

	//	ImGui::EndTable();
	//}
	//ImGui::PopStyleVar();
}

void PanelResource::DrawImageTable(const char* name) {
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::AlignTextToFramePadding();
	bool node_open = ImGui::TreeNode("%s", name);
	ImGui::TableSetColumnIndex(1);
	ImGui::Text((hasResource ? resource->GetResourceFilePath() : ""));

	if (node_open && hasResource) {
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;

		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::AlignTextToFramePadding();
		ImGui::TreeNodeEx("Field", flags, "Field_1");
		ImGui::TableSetColumnIndex(1);
		ImGui::Text("value");
		ImGui::NextColumn();

		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::AlignTextToFramePadding();
		ImGui::TreeNodeEx("Field", flags, "Field_2");
		ImGui::TableSetColumnIndex(1);
		ImGui::Text("value");
	}
	ImGui::TreePop();
}
