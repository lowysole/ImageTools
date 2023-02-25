#include "Application.h"

#include "Globals.h"
#include "Modules/ModuleWindow.h"
#include "Modules/ModuleInput.h"
#include "Modules/ModuleEditor.h"
#include "Modules/ModuleRender.h"

Application::Application() {
	// NOTE: Add all modules from App here. Important Order of creation.
	modules.push_back(window = new ModuleWindow());
	modules.push_back(input = new ModuleInput());
	modules.push_back(editor = new ModuleEditor());
	modules.push_back(render = new ModuleRender());
}

Application::~Application() {
	for (Module* module : modules) {
		delete module;
	}
}

bool Application::Init() {
	bool status = true;
	for (std::vector<Module*>::iterator it = modules.begin(); it != modules.end() && status; ++it) {
		status = (*it)->Init();
	}
	return status;
}

bool Application::Start() {
	bool status = true;
	for (std::vector<Module*>::iterator it = modules.begin(); it != modules.end() && status; ++it) {
		status = (*it)->Start();
	}
	return status;
}

UpdateStatus Application::Update() {
	UpdateStatus status = UpdateStatus::CONTINUE;

	for (std::vector<Module*>::iterator it = modules.begin(); it != modules.end() && status == UpdateStatus::CONTINUE; ++it) {
		status = (*it)->PreUpdate();
	}

	for (std::vector<Module*>::iterator it = modules.begin(); it != modules.end() && status == UpdateStatus::CONTINUE; ++it) {
		status = (*it)->Update();
	}

	for (std::vector<Module*>::iterator it = modules.begin(); it != modules.end() && status == UpdateStatus::CONTINUE; ++it) {
		status = (*it)->PostUpdate();
	}

	return status;
}

bool Application::CleanUp() {
	bool status = true;
	for (std::vector<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && status; ++it) {
		status = (*it)->CleanUp();
	}
	return status;
}
