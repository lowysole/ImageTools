#include "Application.h"

#include "Globals.h"
#include "Utils/Logging.h"

#include "Utils/Leaks.h"

enum class MainState {
	CREATION,
	INIT,
	START,
	UPDATE,
	FINISH,
	EXIT
};

Application* App = nullptr;

int main() {
	// Logging
	logger = new Logger();

	// App Loop
	int mainReturn = EXIT_FAILURE;
	MainState state = MainState::CREATION;
	while (state != MainState::EXIT) {
		switch (state) {
		case MainState::CREATION:
			App = new Application();
			state = MainState::INIT;
			break;

		case MainState::INIT:
			if (App->Init() == false) {
				state = MainState::EXIT;
			} else {
				state = MainState::START;
			}
			break;

		case MainState::START:
			if (App->Start() == false) {
				state = MainState::EXIT;
			} else {
				state = MainState::UPDATE;
			}
			break;

		case MainState::UPDATE: {
			UpdateStatus updateStatus = App->Update();

			if (updateStatus == UpdateStatus::ERROR) {
				state = MainState::EXIT;
			} else if (updateStatus == UpdateStatus::STOP) {
				state = MainState::FINISH;
			}
			break;
		}

		case MainState::FINISH:
			if (App->CleanUp() == false) {
			} else {
				mainReturn = EXIT_SUCCESS;
			}

			state = MainState::EXIT;
			break;
		}
	}

	RELEASE(App);
	RELEASE(logger);

	return mainReturn;
}
