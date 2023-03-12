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
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
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
	ImGui::PopStyleColor(2);
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

	ImGui::SetNextWindowSize(infoTableSize);
	ImGui::SetNextWindowCollapsed(true, ImGuiCond_Appearing);

	ImGui::Begin("Info", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	ImGui::SetWindowFontScale(0.9f);

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
	if (ImGui::BeginTable("##table", 2, ImGuiTableFlags_SizingFixedSame)) {

		ImGui::TableSetupColumn("##col1", ImGuiTableColumnFlags_WidthFixed, 160);
		bool hasResource = resource->HasResource(0);
		const char* path = hasResource ? resource->GetResourceFilePath(0) : "";
		ImGui::TableSetupColumn("##col2", ImGuiTableColumnFlags_WidthStretch, ImGui::CalcTextSize(path).x);
		infoTableSize = 160.0f + ImGui::CalcTextSize(path).x;

		DrawImageTable("Image 1", 0);

		ImGui::EndTable();

		ImGuiContext& g = *GImGui;
		ImGuiTable* table = g.Tables.GetOrAddByKey(ImGui::GetID("##table"));
		if (table->OuterWindow) {
			infoTableSize = float2(table->OuterWindow->ContentSizeIdeal.x + 60, table->OuterWindow->ContentSizeIdeal.y + 60);
		}
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
	}
}

void PanelResource::DrawPanelCompare() {
	ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + 30));

	ImGui::SetNextWindowSize(infoTableSize);
	ImGui::SetNextWindowCollapsed(true, ImGuiCond_Appearing);

	ImGui::Begin("Info", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	ImGui::SetWindowFontScale(0.9f);

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
	if (ImGui::BeginTable("##table", 2, ImGuiTableFlags_SizingFixedSame)) {

		ImGui::TableSetupColumn("##col1", ImGuiTableColumnFlags_WidthFixed, 160);
		bool hasResource = resource->HasResource(0);
		const char* path = hasResource ? resource->GetResourceFilePath(0) : "";
		ImGui::TableSetupColumn("##col2", ImGuiTableColumnFlags_WidthStretch, ImGui::CalcTextSize(path).x);

		ImGui::PushID("it0");
		DrawImageTable("Image 1", 0);
		ImGui::PopID();
		ImGui::PushID("it1");
		DrawImageTable("Image 2", 1);
		ImGui::PopID();

		ImGui::EndTable();

		ImGuiContext& g = *GImGui;
		ImGuiTable* table = g.Tables.GetOrAddByKey(ImGui::GetID("##table"));
		if (table->OuterWindow) {
			infoTableSize = float2(table->OuterWindow->ContentSizeIdeal.x + 60, table->OuterWindow->ContentSizeIdeal.y + 60);
		}
	}

	ImGui::PopStyleVar();
	ImGui::End();

	// Image
	if (resource->HasResource(0)) {
		cv::Mat* image = resource->GetResourceData(0);
		ImVec2 size = ImGui::GetWindowSize();
		float paddingX = 20.0f;
		float width = (size.x - 4 * paddingX) / 3;
		float height = width * image->rows / (float) image->cols;
		float PaddingY = (size.y - height) / 2;

		float image1pos = paddingX;
		ImGui::SetCursorPos(float2(image1pos, PaddingY));
		ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(resource->GetResourceID(0))), ImVec2(width, height));
		float image2pos = 2 * paddingX + width;
		ImGui::SetCursorPos(float2(image2pos, PaddingY));
		ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(resource->GetResourceID(1))), ImVec2(width, height));
		float image3pos = 3 * paddingX + 2 * width;
		ImGui::SetCursorPos(float2(image3pos, PaddingY));
		ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(resource->GetResourceID(2))), ImVec2(width, height));
	}
}

void PanelResource::DrawImageTable(const char* name, uint id) {
	bool hasResource = resource->HasResource(id);
	const char* path = hasResource ? resource->GetResourceFilePath(id) : "";

	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::AlignTextToFramePadding();
	bool node_open = ImGui::TreeNode("%s", name);

	ImGui::TableSetColumnIndex(1);
	ImGui::Text(path);

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
