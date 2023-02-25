#include "PanelResult.h"

#include "Application.h"
#include "Modules/ModuleEditor.h"
#include "Utils/Logging.h"

#include "imgui.h"
#include "IconsFontAwesome5.h"

#include "Utils/Leaks.h"

PanelResult::PanelResult()
	: Panel("Result", true) {
}

void PanelResult::Update() {
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x * 0.2f, viewport->WorkSize.y * 0.75f), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x + 20, viewport->WorkPos.y + viewport->WorkSize.y * 0.75f), ImGuiCond_FirstUseEver);
	std::string windowName = std::string(ICON_FA_EQUALS "  ") + GetName();
	if (ImGui::Begin(windowName.c_str(), &UpdateEnabled())) {
	}
	ImGui::End();
}