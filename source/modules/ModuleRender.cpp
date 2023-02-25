#include "ModuleRender.h"

#include "Globals.h"
#include "Application.h"
#include "Modules/ModuleWindow.h"
#include "Utils/Logging.h"

#include "SDL.h"
#include "GL/glew.h"

bool ModuleRender::Init() {
	LOG("Creating Renderer context");

	context = SDL_GL_CreateContext(App->window->window);

	GLenum err = glewInit();
	LOG("Using Glew %s", glewGetString(GLEW_VERSION));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glEnable(GL_MULTISAMPLE);
	glFrontFace(GL_CCW);

	viewportSize = App->window->GetWindowsSize();
	glViewport(0, 0, static_cast<int>(viewportSize.x), static_cast<int>(viewportSize.y));

	return true;
}

UpdateStatus ModuleRender::Update() {
	if (IsResized()) {
		glViewport(0, 0, static_cast<int>(viewportSize.x), static_cast<int>(viewportSize.y));
	}

	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_ALWAYS);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return UpdateStatus::CONTINUE;
}

UpdateStatus ModuleRender::PostUpdate() {

	SDL_GL_SwapWindow(App->window->window);

	return UpdateStatus::CONTINUE;
}

bool ModuleRender::CleanUp() {
	return true;
}

bool ModuleRender::IsResized() {

	float2 size = App->window->GetWindowsSize();
	if (size.x != viewportSize.x && size.y != viewportSize.y) {
		viewportUpdated = true;
		viewportSize.x = size.x;
		viewportSize.y = size.y;

	} else {
		viewportUpdated = false;
	}

	return viewportUpdated;
}

bool ModuleRender::GetIsResized() const {
	return viewportUpdated;
}

float2 ModuleRender::GetViewportSize() const {
	return viewportSize;
}
