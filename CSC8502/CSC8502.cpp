/******************************************************************************
This file is part of the Newcastle OpenGL Tutorial Series
Author:Rich Davison
Contact:richgdavison@gmail.com
License: MIT (see LICENSE file at the top of the source tree)
*//////////////////////////////////////////////////////////////////////////////
#include "../NCLCoreClasses/Window.h"

#include "TutorialRenderer.h"

using namespace NCL;
using namespace CSC8502;

int main() {
	NCL::WindowInitialisation winInit = {
		.width = 1120,
		.height = 768,
		.windowTitle = "Welcome to CSC8502!",
	};

	Window* w = Window::CreateGameWindow(winInit);

	std::string defaultTutorial = "Tutorial1";

	if (!w->HasInitialised()) {
		return -1;
	}

	auto tutorial = std::unique_ptr<TutorialRenderer>(TutorialRenderer::CreateTutorial(defaultTutorial));

	w->SetConsolePosition(2000, 200);

	w->LockMouseToWindow(true);
	w->ShowOSPointer(false);

	while (w->UpdateWindow() && !Window::GetKeyboard()->KeyDown(KeyCodes::ESCAPE)) {
		float time = w->GetTimer().GetTimeDeltaSeconds();
		tutorial->Update(time);
		tutorial->Render();
	}

	Window::DestroyGameWindow();
	return 0;
}