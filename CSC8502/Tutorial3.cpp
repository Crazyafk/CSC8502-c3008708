/******************************************************************************
This file is part of the Newcastle OpenGL Tutorial Series
Author:Rich Davison
Contact:richgdavison@gmail.com
License: MIT (see LICENSE file at the top of the source tree)
*//////////////////////////////////////////////////////////////////////////////
#include "Tutorial3.h"

using namespace NCL;
using namespace Rendering;
using namespace CSC8502;

TUTORIAL_ENTRY(Tutorial3)

Tutorial3::Tutorial3() : TutorialRenderer(){
	shader = std::make_unique<OGLShader>("Tutorial1.vert", "Tutorial1.frag");
}

void Tutorial3::RenderFrame() {
	BindShader(*shader);
	BindMesh(*triMesh);

	DrawBoundMesh();
}