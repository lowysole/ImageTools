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

void PanelConsole::Update() {
	logger->UpdateLogString();

	std::string windowName = std::string(ICON_FK_TERMINAL "  ") + GetName();
	if (ImGui::Begin(windowName.c_str(), &UpdateEnabled())) {
		// Output
		const float footerHeightToReserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
		ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footerHeightToReserve), false, ImGuiWindowFlags_HorizontalScrollbar);

		if (ImGui::BeginPopupContextWindow()) {
			if (ImGui::Selectable("Clear")) logger->logString.clear();
			ImGui::EndPopup();
		}

		if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
			ImGui::SetScrollHereY(1.0f);
		}
		ImGui::EndChild();

		ImGui::Separator();

		// Command-line
		bool reclaimFocus = false;
		char inputBuf[256] = {0};
		ImGuiInputTextFlags inputTextFlags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
		if (ImGui::InputText("Input", inputBuf, IM_ARRAYSIZE(inputBuf), inputTextFlags, &ExecuteCommand)) {
			logger->logString += "$FFB86CFF # ";
			logger->logString += inputBuf;
			logger->logString += "\n";
			reclaimFocus = true;
		}
		ImGui::SetItemDefaultFocus();
		if (reclaimFocus) {
			ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget
		}
	}
	ImGui::End();


}
