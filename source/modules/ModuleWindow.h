#pragma once

#include "Modules/Module.h"

#include "Resources/Math/float2.h"

struct SDL_Window;
struct SDL_Surface;

class ModuleWindow : public Module {
public:
	bool Init() override;
	bool CleanUp() override;

	float2 GetWindowsSize();

public:
	SDL_Window* window = nullptr;
	SDL_Surface* surface = nullptr;
};
