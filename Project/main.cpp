/******************************************************************************
This file is part of the Newcastle Vulkan Tutorial Series
Author:Rich Davison
Contact:richgdavison@gmail.com
License: MIT (see LICENSE file at the top of the source tree)
*//////////////////////////////////////////////////////////////////////////////
#include "../NCLCoreClasses/Window.h"
#include "../NCLCoreClasses/GameTimer.h"
#include "Renderer.h"

using namespace NCL;
using namespace Rendering;
using namespace Project;

int main(int argc, char* argv[]) {
	NCL::WindowInitialisation winInit = {
		.width = 1120,
		.height = 768,
		.windowTitle = "Welcome to OpenGL!",
	};

	bool lockCursor = false;

	for (int i = 0; i < argc; ++i) {
		std::string argument(argv[i]);
		for (char& c : argument) c = (char)tolower(c);
		if (argument == "-screenwidth" || argument == "-w") {
			winInit.width = std::stoi(argv[++i]);
		}
		else if (argument == "-screenheight" || argument == "-h") {
			winInit.height = std::stoi(argv[++i]);
		}
		else if (argument == "-screenx" || argument == "-x") {
			winInit.windowPositionX = std::stoi(argv[++i]);
		}
		else if (argument == "-screeny" || argument == "-y") {
			winInit.windowPositionY = std::stoi(argv[++i]);
		}
		else if (argument == "-consolex") {
			winInit.consolePositionX = std::stoi(argv[++i]);
		}
		else if (argument == "-consoley") {
			winInit.consolePositionY = std::stoi(argv[++i]);
		}
		else if (argument == "-refreshrate") {
			winInit.refreshRate = std::stoi(argv[++i]);
		}
		else if (argument == "-fullscreen") {
			winInit.fullScreen = true;
		}
		else if (argument == "-lockcursor") {
			lockCursor = true;
		}
	}

	Window* w = Window::CreateGameWindow(winInit);

	if (!w->HasInitialised()) {
		return -1;
	}

	w->LockMouseToWindow(lockCursor);

	std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>();

	w->SetWindowEventHandler(
		[&](WindowEvent e, uint32_t w, uint32_t h) {
			renderer->WindowEventHandler(e, w, h);
		}
	);

	while (w->UpdateWindow() && !Window::GetKeyboard()->KeyDown(KeyCodes::ESCAPE)) {
		float time = w->GetTimer().GetTimeDeltaSeconds();
		if (time > 0.25f) {
			continue; //Skip large dts
		}
		renderer->Update(time);
		renderer->Render();
	}

	Window::DestroyGameWindow();

	return 0;
}