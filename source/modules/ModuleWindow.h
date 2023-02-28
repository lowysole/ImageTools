#pragma once

#include "Modules/Module.h"

struct SDL_Window;
struct SDL_Surface;
class float2;

class ModuleWindow : public Module {
public:
	bool Init() override;
	bool CleanUp() override;

	float2 GetWindowsSize();

public:
	SDL_Window* window = nullptr;
	SDL_Surface* surface = nullptr;
};
