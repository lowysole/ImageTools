#include "ModuleEditor.h"

#include "Globals.h"
#include "Application.h"
#include "Modules/ModuleWindow.h"
#include "Modules/ModuleRender.h"

#include "GL/glew.h"
#include "SDL.h"
#include "imgui_internal.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "IconsFontAwesome5.h"
#include "IconsForkAwesome.h"

#include "Utils/Leaks.h"

static const ImWchar iconsRangesFa[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
static const ImWchar iconsRangesFk[] = {ICON_MIN_FK, ICON_MAX_FK, 0};

bool ModuleEditor::Init() {
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
	io.ConfigWindowsMoveFromTitleBarOnly = true;

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		io.ConfigViewportsNoAutoMerge = false;
		io.ConfigViewportsNoTaskBarIcon = true;
	}

	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
		io.ConfigDockingTransparentPayload = true;
	}

	SetImGuiTheme();

	// Set Panels
	panels.push_back(&panelAbout);
	panels.push_back(&panelOperations);
	panels.push_back(&panelResource);
	panels.push_back(&panelResult);
	panels.push_back(&panelConsole);

	return true;
}

bool ModuleEditor::Start() {
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->render->context);
	ImGui_ImplOpenGL3_Init(GLSL_VERSION);

	ImGui::StyleColorsDark();

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	workPos = viewport->WorkPos;
	workSize = viewport->WorkSize;

	return true;
}

UpdateStatus ModuleEditor::PreUpdate() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	//ImGuiViewport* viewport = ImGui::GetMainViewport();
	//if (viewport->WorkSize.x != workSize.x || viewport->WorkSize.y != workSize.y) {
	//	isResized = true;
	//}

	return UpdateStatus::CONTINUE;
}

UpdateStatus ModuleEditor::Update() {
	ImGui::ShowDemoWindow();

	// Main Menu bar
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("File")) {
		if (ImGui::MenuItem("Quit")) {
			modalToOpen = Modal::QUIT;
		}
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Windows")) {
		ImGui::MenuItem(panelOperations.GetName(), "", &panelOperations.UpdateEnabled());
		ImGui::MenuItem(panelResult.GetName(), "", &panelResult.UpdateEnabled());
		ImGui::MenuItem(panelConsole.GetName(), "", &panelConsole.UpdateEnabled());
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Help")) {
		ImGui::MenuItem(panelAbout.GetName(), "", &panelAbout.UpdateEnabled());
		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();

	// Modals
	switch (modalToOpen) {
	case Modal::QUIT:
		ImGui::OpenPopup("Quit");
		break;
	}
	modalToOpen = Modal::NONE;

	// Quit Modal
	ImGui::SetNextWindowSize(ImVec2(300, 100), ImGuiCond_FirstUseEver);
	if (ImGui::BeginPopupModal("Quit", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar)) {
		ImGui::Text("Do you really want to quit?");
		ImGui::NewLine();
		ImGui::NewLine();
		ImGui::SameLine(ImGui::GetWindowWidth() - 140);
		if (ImGui::Button("Quit", ImVec2(60, 20))) {
			return UpdateStatus::STOP;
		}
		ImGui::SameLine(ImGui::GetWindowWidth() - 70);
		if (ImGui::Button("Cancel", ImVec2(60, 20))) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	for (Panel* panel : panels) {
		if (panel->GetEnabled()) panel->Update();
	}

	return UpdateStatus::CONTINUE;
}

UpdateStatus ModuleEditor::PostUpdate() {
	// Render main window
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Handle and render other windows
	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
	SDL_GL_MakeCurrent(App->window->window, App->render->context);

	//if (isResized) {
	//	ImGuiViewport* viewport = ImGui::GetMainViewport();
	//	isResized = false;
	//	workSize.x = viewport->WorkSize.x;
	//	workSize.y = viewport->WorkSize.y;
	//}

	return UpdateStatus::CONTINUE;
}

bool ModuleEditor::CleanUp() {
	panels.clear();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}

bool ModuleEditor::GetIsResized() const {
	return isResized;
}

ImVec2 ModuleEditor::GetWorkSize() const {
	return workSize;
}

void ModuleEditor::SetImGuiTheme() {
	//Font
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("Resources/Fonts/Gravity_Book.otf", 18.0f);
	ImFontConfig iconsConfig;
	iconsConfig.MergeMode = true;
	iconsConfig.PixelSnapH = true;
	io.Fonts->AddFontFromFileTTF("Resources/Fonts/" FONT_ICON_FILE_NAME_FAS, 12.0f, &iconsConfig, iconsRangesFa);
	io.Fonts->AddFontFromFileTTF("Resources/Fonts/" FONT_ICON_FILE_NAME_FK, 12.0f, &iconsConfig, iconsRangesFk);


	// Colors
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.36f, 0.42f, 0.47f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.20f, 0.28f, 1.00f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.09f, 0.12f, 0.14f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.12f, 0.14f, 0.65f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.39f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.18f, 0.22f, 0.25f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.09f, 0.21f, 0.31f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.37f, 0.61f, 1.00f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.20f, 0.25f, 0.29f, 0.55f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_Tab] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

	// Style
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowPadding = ImVec2(16.00f, 16.00f);
	style.FramePadding = ImVec2(10.00f, 4.00f);
	style.CellPadding = ImVec2(8.00f, 2.00f);
	style.ItemSpacing = ImVec2(12.00f, 4.00f);
	style.ItemInnerSpacing = ImVec2(10.00f, 6.00f);
	style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
	style.IndentSpacing = 25;
	style.ScrollbarSize = 15;
	style.GrabMinSize = 12;
	style.WindowBorderSize = 0;
	style.ChildBorderSize = 0;
	style.PopupBorderSize = 1;
	style.FrameBorderSize = 0;
	style.TabBorderSize = 0;
	style.WindowRounding = 7;
	style.ChildRounding = 4;
	style.FrameRounding = 12;
	style.PopupRounding = 12;
	style.ScrollbarRounding = 12;
	style.GrabRounding = 3;
	style.LogSliderDeadzone = 4;
	style.TabRounding = 4;
}
