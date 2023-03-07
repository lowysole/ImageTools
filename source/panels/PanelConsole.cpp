#include "PanelConsole.h"

#include "Application.h"
#include "Modules/ModuleEditor.h"
#include "Utils/Logging.h"

#include "imgui.h"
#include "IconsForkAwesome.h"

#include "Utils/Leaks.h"

static int ExecuteCommand(ImGuiInputTextCallbackData* data) {
	return 0;
}

PanelConsole::PanelConsole()
	: Panel("Console", true) {
}

void PanelConsole::DrawPanel() {
	logger->UpdateLogString();
	ImGui::SetNextWindowDockID(App->editor->dockDownId, ImGuiCond_FirstUseEver);
	std::string windowName = std::string(ICON_FK_TERMINAL "  ") + GetName();
	if (ImGui::Begin(windowName.c_str(), &UpdateEnabled())) {
		// Output
		ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

		if (ImGui::BeginPopupContextWindow()) {
			if (ImGui::Selectable("Clear")) logger->logString.clear();
			ImGui::EndPopup();
		}

		ImGui::Text(logger->logString.c_str());
		if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
			ImGui::SetScrollHereY(1.0f);
		}
		ImGui::EndChild();
	}
	ImGui::End();


}
