#include "PanelResource.h"

#include "Application.h"
#include "Modules/ModuleEditor.h"
#include "Resources/Resource.h"
#include "Panels/PanelInspector.h"
#include "Utils/Logging.h"

#include "GL/glew.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "IconsFontAwesome5.h"
#include "opencv2/core/mat.hpp"

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

PanelResource::~PanelResource() {
	RELEASE(resource);
}

void PanelResource::DrawPanel() {
	ImGui::SetNextWindowDockID(App->editor->dockMainId, ImGuiCond_FirstUseEver);
	std::string windowName = std::string(ICON_FA_IMAGE "  ") + GetName();
	if (ImGui::Begin(windowName.c_str(), &UpdateEnabled()), ImGuiWindowFlags_AlwaysAutoResize) {
		if (ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows)) {
			App->editor->panelInspector.SetPanelResourceSelected(this);
		}

		ImGuiWindow* window = ImGui::GetCurrentWindowRead();
		if (!window->DockIsActive || window->DockTabIsVisible) {
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
		}

		ImGui::End();
	}
}

Resource* PanelResource::GetResource() const {
	return resource;
}

PanelResourceType PanelResource::GetPanelResourceType() const {
	return panelResourceType;
}

void PanelResource::DrawPanelDefault() {
	ImGui::Text("Welcome to ImageTools!");
}

void PanelResource::DrawPanelImage() {
	ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + 30));
	ImGui::SetNextWindowCollapsed(true, ImGuiCond_Appearing);
	ImGui::Begin("Info", NULL, ImGuiWindowFlags_NoMove);
	ImGui::SetWindowFontScale(0.9f);

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
	if (ImGui::BeginTable("##table1", 2, ImGuiTableFlags_SizingFixedSame)) {
		DrawImageTable("Image 1");

		ImGui::EndTable();
	}

	ImGui::PopStyleVar();
	ImGui::End();

	// Image
	if (resource->HasResource()) {
		cv::Mat* image = resource->GetResourceData();

		ImVec2 size = ImGui::GetWindowSize();
		float width = 0.33f * size.x;
		float height = width * image->rows / (float) image->cols;

		ImGui::SetCursorPos((float2(ImGui::GetContentRegionAvail()) - float2(width, height)) * 0.5f);
		ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(resource->GetResourceID())), ImVec2(width, height));

		ImGui::SetScrollX(500);
		ImGui::SetScrollY(500);
	}
}

void PanelResource::DrawPanelCompare() {
	//ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + 30));
	//ImGui::SetNextWindowCollapsed(true, ImGuiCond_Appearing);
	//ImGui::Begin("Info", NULL, ImGuiWindowFlags_NoMove);
	//ImGui::SetWindowFontScale(0.9f);

	//ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
	//if (ImGui::BeginTable("##table1", 2, ImGuiTableFlags_SizingFixedSame)) {
	//	DrawImageTable("Image 1");
	//	DrawImageTable("Image 2");
	//	ImGui::EndTable();
	//}

	//ImGui::PopStyleVar();
	//ImGui::End();

	//// Image
	//if (resource->HasResource()) {
	//	cv::Mat* image1 = resource->GetResourceData();

	//	ImVec2 size = ImGui::GetWindowSize();
	//	float width = 0.33f * size.x;
	//	float height = width * image->rows / (float) image->cols;

	//	ImGui::SetCursorPos((float2(ImGui::GetContentRegionAvail()) - float2(width, height)) * 0.5f);
	//	ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(resource->GetResourceID())), ImVec2(width, height));

	//	ImGui::SetScrollX(500);
	//	ImGui::SetScrollY(500);
	//}
}

void PanelResource::DrawImageTable(const char* name) {
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::AlignTextToFramePadding();
	bool node_open = ImGui::TreeNode("%s", name);
	bool hasResource = resource->HasResource();
	ImGui::TableSetColumnIndex(1);
	ImGui::Text((hasResource ? resource->GetResourceFilePath() : ""));

	if (node_open) {
		if (!hasResource) {
			ImGui::TreePop();
			return;
		}

		cv::Mat* resourceData = resource->GetResourceData();

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;

		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::AlignTextToFramePadding();

		ImGui::TreeNodeEx("Size", flags);
		ImGui::TableSetColumnIndex(1);
		ImGui::Text("%d x %d", resourceData->cols, resourceData->rows);
		ImGui::NextColumn();

		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::AlignTextToFramePadding();
		ImGui::TreeNodeEx("Channels", flags);
		ImGui::TableSetColumnIndex(1);
		ImGui::Text("%d", resourceData->channels());

		ImGui::TreePop();
	}
}
