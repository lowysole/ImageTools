#pragma once

#include "Modules/Module.h"
#include "Resources/Math/float2.h"

#include "SDL_scancode.h"

#define NUM_MOUSE_BUTTONS 5

enum class KeyState : int {
	KS_IDLE = 0,
	KS_DOWN,
	KS_REPEAT,
	KS_UP
};

class ModuleInput : public Module {
public:
	bool Init() override;
	UpdateStatus PreUpdate() override;
	bool CleanUp() override;

private:
	KeyState keyboard[SDL_NUM_SCANCODES] = {KeyState::KS_IDLE};		// Array that stores the 'KeyState' of every key in the keyboard. See KeyState for possible states.
	KeyState mouseButtons[NUM_MOUSE_BUTTONS] = {KeyState::KS_IDLE}; // Same keystate, but for the mouse buttons.
	float mouseWheelMotion = 0;										// Stores the increment registered by the mouse wheel on a frame.
	float2 mouseMotion = {0, 0};									// Stores de movement increment of the mouse position on a frame.
	float2 mouse = {0, 0};											// Stores the mouse position.
	bool mouseWarped = false;										// "Flag" to indicate that the mouse has been warped. When warping happens and it is set to true, the PreUpdate() will ignore the mouse motion for one frame.
};
