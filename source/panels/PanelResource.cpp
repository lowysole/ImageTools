#include "PanelResource.h"

#include "Application.h"
#include "Modules/ModuleEditor.h"
#include "Modules/ModuleTexture.h"
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
	DeletePanel();
}

void PanelResource::DrawPanel() {
	ImGui::SetNextWindowDockID(App->editor->dockMainId, ImGuiCond_FirstUseEver);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
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
	ImGui::PopStyleColor();
}

Resource* PanelResource::GetResource() const {
	return resource;
}

PanelResourceType PanelResource::GetPanelResourceType() const {
	return panelResourceType;
}

void PanelResource::DeletePanel() {
	RELEASE(resource);
}

void PanelResource::DrawPanelDefault() {
	ImGui::Text("Welcome to ImageTools!");
}

void PanelResource::DrawPanelImage() {
	ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + 30));
	ImGui::SetNextWindowCollapsed(true, ImGuiCond_Appearing);
	ImGui::Begin("Info", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_HorizontalScrollbar);
	ImGui::SetWindowFontScale(0.9f);

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
	if (ImGui::BeginTable("##table", 2, ImGuiTableFlags_SizingFixedSame)) {
		DrawImageTable("Image 1", 0);

		ImGui::EndTable();
	}

	ImGui::PopStyleVar();
	ImGui::End();

	// Image
	if (resource->HasResource(0)) {
		cv::Mat* image = resource->GetResourceData(0);

		ImVec2 size = ImGui::GetWindowSize();
		float width = 0.33f * size.x;
		float height = width * image->rows / (float) image->cols;

		ImGui::SetCursorPos((float2(ImGui::GetContentRegionAvail()) - float2(width, height)) * 0.5f);
		ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(resource->GetResourceID(0))), ImVec2(width, height), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(0.5, 0.5, 0.5, 0.5));

		ImGui::SetScrollX(500);
		ImGui::SetScrollY(500);
	}
}

void PanelResource::DrawPanelCompare() {
	ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + 30));
	ImGui::SetNextWindowCollapsed(true, ImGuiCond_Appearing);
	ImGui::Begin("Info", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_HorizontalScrollbar);
	ImGui::SetWindowFontScale(0.9f);

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
	if (ImGui::BeginTable("##table", 2, ImGuiTableFlags_SizingFixedSame)) {
		ImGui::PushID("it0");
		DrawImageTable("Image 1", 0);
		ImGui::PopID();
		ImGui::PushID("it1");
		DrawImageTable("Image 2", 1);
		ImGui::PopID();

		ImGui::EndTable();
	}

	ImGui::PopStyleVar();
	ImGui::End();

	// Image
	if (resource->HasResource(0)) {
		cv::Mat* image = resource->GetResourceData(0);

		ImVec2 size = ImGui::GetWindowSize();
		float width = 0.33f * size.x;
		float height = width * image->rows / (float) image->cols;

		ImGui::SetCursorPos((float2(ImGui::GetContentRegionAvail()) - float2(width, height)) * 0.5f);
		ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(resource->GetResourceID(0))), ImVec2(width, height));

		ImGui::SetScrollX(500);
		ImGui::SetScrollY(500);
	}
}

void PanelResource::DrawImageTable(const char* name, uint id) {
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::AlignTextToFramePadding();
	bool node_open = ImGui::TreeNode("%s", name);
	bool hasResource = resource->HasResource(id);
	ImGui::TableSetColumnIndex(1);
	ImGui::Text((hasResource ? resource->GetResourceFilePath(id) : ""));

	if (node_open) {
		if (!hasResource) {
			ImGui::TreePop();
			return;
		}

		cv::Mat* resourceData = resource->GetResourceData(id);

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
		ImGui::Text("%d (%s)", resourceData->channels(), ModuleTexture::GetTextureFormatToString(resourceData->channels()).c_str());
		ImGui::NextColumn();

		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::AlignTextToFramePadding();
		ImGui::TreeNodeEx("Type", flags);
		ImGui::TableSetColumnIndex(1);
		ImGui::Text("%s", ModuleTexture::GetTextureTypeToString(resourceData->type()).c_str());
		ImGui::NextColumn();

		ImGui::TreePop();
	}
}
