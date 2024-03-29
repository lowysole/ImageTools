#include "ModuleInput.h"

#include "Globals.h"
#include "Application.h"
#include "Modules/ModuleWindow.h"
#include "Utils/Logging.h"

#include "SDL.h"
#include "imgui_impl_sdl.h"

#include "Utils/Leaks.h"

bool ModuleInput::Init() {
	bool ret = true;
	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0) {
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

UpdateStatus ModuleInput::PreUpdate() {

	ImGuiIO& io = ImGui::GetIO();

	mouseMotion = float2::zero;
	mouseWheelMotion = 0;

	int windowId = SDL_GetWindowID(App->window->window);

	for (int i = 0; i < SDL_NUM_SCANCODES; ++i) {
		if (keyboard[i] == KeyState::KS_DOWN) {
			keyboard[i] = KeyState::KS_REPEAT;
		}

		if (keyboard[i] == KeyState::KS_UP) {
			keyboard[i] = KeyState::KS_IDLE;
		}
	}

	for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i) {
		if (mouseButtons[i] == KeyState::KS_DOWN) {
			mouseButtons[i] = KeyState::KS_REPEAT;
		}

		if (mouseButtons[i] == KeyState::KS_UP) {
			mouseButtons[i] = KeyState::KS_IDLE;
		}
	}

	int auxMouseX;
	int auxMouseY;
	SDL_GetGlobalMouseState(&auxMouseX, &auxMouseY);

	SDL_Event event;
	while (SDL_PollEvent(&event) != 0) {
		ImGui_ImplSDL2_ProcessEvent(&event);

		switch (event.type) {
		case SDL_QUIT:
			return UpdateStatus::STOP;
		case SDL_WINDOWEVENT:
			if (event.window.windowID == windowId) {
				switch (event.window.event) {
				case SDL_WINDOWEVENT_CLOSE:
					return UpdateStatus::STOP;
				case SDL_WINDOWEVENT_RESIZED:
					// TODO: Resize
					break;
				}
			}
			break;

		case SDL_MOUSEWHEEL:
			if (event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED) {
				mouseWheelMotion = (float) event.wheel.x;
			} else {
				mouseWheelMotion = (float) event.wheel.y;
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			mouseButtons[event.button.button - 1] = KeyState::KS_DOWN;
			if (event.button.button == SDL_BUTTON_LEFT) {
				// TODO : App->editor->OnMouseClicked();
			}
			break;
		case SDL_MOUSEBUTTONUP:
			mouseButtons[event.button.button - 1] = KeyState::KS_UP;
			if (event.button.button == SDL_BUTTON_LEFT) {
				// TODO: App->editor->OnMouseReleased();
			}
			break;

		case SDL_KEYDOWN:
			keyboard[event.key.keysym.scancode] = KeyState::KS_DOWN;
			break;

		case SDL_KEYUP:
			keyboard[event.key.keysym.scancode] = KeyState::KS_UP;
			break;
		}
	}

	if (io.WantCaptureKeyboard) {
		for (int i = 0; i < SDL_NUM_SCANCODES; ++i) {
			keyboard[i] = KeyState::KS_IDLE;
		}
	}

	if (io.WantCaptureMouse) {
		mouseWheelMotion = 0;
		mouseMotion.x = 0;
		mouseMotion.y = 0;

		for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i) {
			mouseButtons[i] = KeyState::KS_IDLE;
		}
	} else {
		int mouseX;
		int mouseY;
		SDL_GetGlobalMouseState(&mouseX, &mouseY);
		if (!mouseWarped) {
			mouseMotion.x = mouseX - mouse.x;
			mouseMotion.y = mouseY - mouse.y;
		} else {
			mouseWarped = false;
		}
		mouse.x = (float) mouseX;
		mouse.y = (float) mouseY;
	}

	return UpdateStatus::CONTINUE;
}

bool ModuleInput::CleanUp() {
	LOG("Quitting SDL input event subsystem");

	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}