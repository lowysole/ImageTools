#include "ModuleEditor.h"

#include "Globals.h"
#include "Application.h"
#include "Modules/ModuleWindow.h"
#include "Modules/ModuleRender.h"
#include "Panels/PanelResource.h"
#include "Resources/Resource.h"
#include "Utils/FileSystem.h"
#include "Utils/portable-file-dialogs.h"

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
	remove("imgui.ini");

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

	// Set ImGui Visual configs
	SetImGuiTheme();

	// Set Panels
	panels.push_back(&panelAbout);
	panels.push_back(&panelInspector);
	panels.push_back(&panelConsole);
	panels.push_back(&panelResource);

	// Default PanelResource Selected
	panelInspector.SetPanelResourceSelected(&panelResource);

	return true;
}

bool ModuleEditor::Start() {
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->render->context);
	ImGui_ImplOpenGL3_Init(GLSL_VERSION);

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	workPos = viewport->WorkPos;
	workSize = viewport->WorkSize;

	return true;
}

UpdateStatus ModuleEditor::PreUpdate() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	return UpdateStatus::CONTINUE;
}

UpdateStatus ModuleEditor::Update() {
	//ImGui::ShowDemoWindow();

	// Main Menu bar
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("File")) {
		// Export
		bool hasResource = panelInspector.GetPanelResourceSelected()->GetPanelResourceType() != PanelResourceType::NONE;
		ImGui::BeginDisabled(!hasResource);
		if (ImGui::MenuItem(std::string(ICON_FA_FILE_EXPORT "    Export...").c_str())) {
			modalToOpen = Modal::EXPORT;
		}
		ImGui::EndDisabled();
		ImGui::Separator();
		// Quit
		if (ImGui::MenuItem("Quit")) {
			modalToOpen = Modal::QUIT;
		}
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Windows")) {
		ImGui::MenuItem(panelInspector.GetName(), "", &panelInspector.UpdateEnabled());
		ImGui::MenuItem(panelConsole.GetName(), "", &panelConsole.UpdateEnabled());
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Resources")) {
		if (ImGui::MenuItem("Add Image")) {
			CreatePanelResource(PanelResourceType::IMAGE);
		}

		if (ImGui::MenuItem("Add Compare Image")) {
			CreatePanelResource(PanelResourceType::COMPARE_IMAGE);
		}
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Help")) {
		ImGui::MenuItem(panelAbout.GetName(), "", &panelAbout.UpdateEnabled());
		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();

	// Modals
	switch (modalToOpen) {
	case Modal::EXPORT:
		ImGui::OpenPopup("Export##modal_export");
		break;
	case Modal::QUIT:
		ImGui::OpenPopup("Quit##modal_quit");
		break;
	}

	// Export Modal
	ImGui::SetNextWindowSize(ImVec2(400, 150));
	if (ImGui::BeginPopupModal("Export##modal_export", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar)) {
		PanelResource *panelSelected = panelInspector.GetPanelResourceSelected();
		uint id = panelSelected->GetPanelResourceType() == PanelResourceType::IMAGE ? 1 : 2;
		Resource* resource = panelSelected->GetResource();
		const char* directoryPath = GetDirPath(resource->GetResourceFilePath(1)).c_str();
		const char* fileName = GetFileNameNoExtension(resource->GetResourceName(id)).c_str();

		const char* comboCompareTypes[] = {".png", ".jpeg"};
		static int format = 0;
		ImGui::Combo("Format file", &format, comboCompareTypes, IM_ARRAYSIZE(comboCompareTypes));

		ImGui::NewLine();
		ImGui::NewLine();
		ImGui::SameLine(ImGui::GetWindowWidth() - 150);
		if (ImGui::Button("Save", ImVec2(65, 25))) {
			auto f = pfd::save_file("Choose file to save", directoryPath + pfd::path::separator() + fileName + comboCompareTypes[format], {"All Files", "*"}, pfd::opt::force_overwrite).result();
			if (!f.empty()) {
				resource->SaveImage(f.c_str(), id);
			}
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine(ImGui::GetWindowWidth() - 80);
		if (ImGui::Button("Cancel", ImVec2(65, 25))) {
			ImGui::CloseCurrentPopup();
		}

		modalToOpen = Modal::NONE;
		ImGui::EndPopup();
	}
	

	// Quit Modal
	ImGui::SetNextWindowSize(ImVec2(300, 120));
	if (ImGui::BeginPopupModal("Quit##modal_quit", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar)) {
		ImGui::Text("Do you really want to quit?");
		ImGui::NewLine();
		ImGui::NewLine();
		ImGui::SameLine(ImGui::GetWindowWidth() - 150);
		if (ImGui::Button("Quit", ImVec2(65, 25))) {
			return UpdateStatus::STOP;
		}
		ImGui::SameLine(ImGui::GetWindowWidth() - 80);
		if (ImGui::Button("Cancel", ImVec2(65, 25))) {
			ImGui::CloseCurrentPopup();
			modalToOpen = Modal::NONE;
		}
		ImGui::EndPopup();
	}

	// Docking
	ImGui::PushID("dockSpace");
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGuiID dockSpaceId = ImGui::GetID("DockSpace");

	if (!ImGui::DockBuilderGetNode(dockSpaceId)) {
		ImGui::DockBuilderAddNode(dockSpaceId);
		ImGui::DockBuilderSetNodeSize(dockSpaceId, viewport->WorkSize);

		dockMainId = dockSpaceId;

		dockRightId = ImGui::DockBuilderSplitNode(dockMainId, ImGuiDir_Right, 0.2f, nullptr, &dockMainId);
		dockDownId = ImGui::DockBuilderSplitNode(dockMainId, ImGuiDir_Down, 0.1f, nullptr, &dockMainId);
	}

	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);

	ImGuiWindowFlags dockSpaceWindowFlags = 0;
	dockSpaceWindowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking;
	dockSpaceWindowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace", nullptr, dockSpaceWindowFlags);
	ImGui::PopStyleVar(3);
	ImGui::DockSpace(dockSpaceId);
	ImGui::End();
	ImGui::PopID();

	for (Panel* panel : panels) {
		if (panel->GetEnabled()) panel->DrawPanel();
	}

	for (int i = panelsResource.size() - 1; i >= 0; i--) {
		Panel* panel = panelsResource.at(i);
		if (panel->GetEnabled()) {
			panel->DrawPanel();
		} else {
			if (panelInspector.GetPanelResourceSelected() == panel) {
				panelInspector.SetPanelResourceSelected(&panelResource);
			}
			panel->DeletePanel();
			RELEASE(panel);
			panelsResource.erase(panelsResource.begin() + i);
		}
	}

	if (panelsResource.size() == 0) {
		panelResource.SetEnabled(true);
	} else {
		panelResource.SetEnabled(false);
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

void ModuleEditor::CreatePanelResource(PanelResourceType resourceType) {
	char name[25];
	if (resourceType == PanelResourceType::IMAGE) {
		sprintf_s(name, "<empty_%d>", panelsResource.size() + 1);
		panelsResource.push_back(new PanelResource(name, resourceType));
	} else if (resourceType == PanelResourceType::COMPARE_IMAGE) {
		sprintf_s(name, "Compare_%d", panelsResource.size() + 1);
		panelsResource.push_back(new PanelResource(name, resourceType));
	}
}

void ModuleEditor::SetImGuiTheme() {
	//Font
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("data/fonts/Gravity_Book.otf", 18.0f);
	ImFontConfig iconsConfig;
	iconsConfig.MergeMode = true;
	iconsConfig.PixelSnapH = true;
	io.Fonts->AddFontFromFileTTF("data/fonts/" FONT_ICON_FILE_NAME_FAS, 12.0f, &iconsConfig, iconsRangesFa);
	io.Fonts->AddFontFromFileTTF("data/fonts/" FONT_ICON_FILE_NAME_FK, 12.0f, &iconsConfig, iconsRangesFk);

	// Colors
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.33f, 0.33f, 0.33f, 0.92f);
	colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.43f, 0.29f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.33f, 0.67f, 0.86f, 0.72f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
	colors[ImGuiCol_Button] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.23f, 0.23f, 0.23f, 0.54f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
	colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
	colors[ImGuiCol_Tab] = ImVec4(0.46f, 0.46f, 0.46f, 0.52f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_TabActive] = ImVec4(0.71f, 0.71f, 0.71f, 0.52f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_DockingPreview] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
	colors[ImGuiCol_DockingEmptyBg] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_TableBorderLight] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
	colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.23f, 0.23f, 0.23f, 0.53f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.23f, 0.23f, 0.23f, 0.53f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.23f, 0.23f, 0.23f, 0.53f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.23f, 0.23f, 0.23f, 0.53f);

	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowPadding = ImVec2(11.00f, 6.00f);
	style.FramePadding = ImVec2(8.00f, 3.00f);
	style.CellPadding = ImVec2(6.00f, 6.00f);
	style.ItemSpacing = ImVec2(10.00f, 6.00f);
	style.ItemInnerSpacing = ImVec2(6.00f, 6.00f);
	style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
	style.IndentSpacing = 25;
	style.ScrollbarSize = 15;
	style.GrabMinSize = 10;
	style.WindowBorderSize = 1;
	style.ChildBorderSize = 1;
	style.PopupBorderSize = 1;
	style.FrameBorderSize = 0;
	style.TabBorderSize = 0;
	style.WindowRounding = 7;
	style.ChildRounding = 4;
	style.FrameRounding = 12;
	style.PopupRounding = 4;
	style.ScrollbarRounding = 9;
	style.GrabRounding = 3;
	style.LogSliderDeadzone = 4;
	style.TabRounding = 4;
}
