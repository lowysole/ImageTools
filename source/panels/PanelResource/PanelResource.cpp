#include "PanelResource.h"

#include "Application.h"
#include "Modules/ModuleEditor.h"
#include "Utils/Logging.h"

#include "imgui.h"
#include "IconsFontAwesome5.h"

#include "Utils/Leaks.h"

PanelResource::PanelResource()
	: Panel("< empty >", true) {
}

PanelResource::PanelResource(const char* _resourceName, PanelResourceType _panelResourceType)
	: Panel(_resourceName, true) {
	panelResourceType = _panelResourceType;
}

void PanelResource::DrawPanel() {
	ImGui::SetNextWindowDockID(App->editor->dockMainId, ImGuiCond_FirstUseEver);

	std::string windowName = std::string(ICON_FA_IMAGE "  ") + GetName();
	if (ImGui::Begin(windowName.c_str(), &UpdateEnabled()), ImGuiWindowFlags_AlwaysAutoResize) {
	}
	ImGui::End();
}

const char* PanelResource::GetPanelName() {
	return panelName;
}

void PanelResource::SetPanelName() {
	panelName = GetResource()->GetResourceName();
}

Resource* PanelResource::GetResource() {
	return resource;
}

void PanelResource::SetResource(Resource* _resource) {
	resource = _resource;
}