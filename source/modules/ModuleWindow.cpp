#include "ModuleWindow.h"

#include "Globals.h"
#include "Application.h"
#include "Utils/Logging.h"

#include "SDL.h"
#include "GL/glew.h"

#include "Utils/Leaks.h"

bool ModuleWindow::Init() {
	LOG("Init SDL window & surface");

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4); // desired version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	Uint32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

	SDL_DisplayMode desktopDisplayMode;
	SDL_GetDesktopDisplayMode(0, &desktopDisplayMode);
	window = SDL_CreateWindow(App->appName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, desktopDisplayMode.w * 0.75f, desktopDisplayMode.h * 0.75f, flags);
	if (window == NULL) {
		LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	surface = SDL_GetWindowSurface(window);

	return true;
}

bool ModuleWindow::CleanUp() {
	LOG("Destroying SDL window and quitting all SDL systems");

	if (window != NULL) {
		SDL_DestroyWindow(window);
	}

	SDL_Quit();
	return true;
}

float2 ModuleWindow::GetWindowsSize() {
	int w;
	int h;
	SDL_GetWindowSize(window, &w, &h);

	return float2(w, h);
}
