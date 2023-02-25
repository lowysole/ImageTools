#pragma once

#include <vector>

enum class UpdateStatus;

class Module;
class ModuleWindow;
class ModuleInput;
class ModuleEditor;
class ModuleRender;

class Application {
public:
	Application();
	~Application();

	bool Init();
	bool Start();
	UpdateStatus Update();
	bool CleanUp();

public:
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleEditor* editor = nullptr;
	ModuleRender* render = nullptr;

	char appName[20] = "MathLib";

private:
	std::vector<Module*> modules;
};

extern Application* App;
