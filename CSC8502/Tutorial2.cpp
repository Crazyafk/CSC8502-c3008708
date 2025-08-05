/******************************************************************************
This file is part of the Newcastle OpenGL Tutorial Series
Author:Rich Davison
Contact:richgdavison@gmail.com
License: MIT (see LICENSE file at the top of the source tree)
*//////////////////////////////////////////////////////////////////////////////
#include "Tutorial2.h"

using namespace NCL;
using namespace Rendering;
using namespace CSC8502;

TUTORIAL_ENTRY(Tutorial2)

Tutorial2::Tutorial2() : TutorialRenderer(){
	shader = std::make_unique<OGLShader>("Tutorial1.vert", "Tutorial1.frag");
}

void Tutorial2::RenderFrame() {
	BindShader(*shader);
	BindMesh(*triMesh);

	DrawBoundMesh();
}