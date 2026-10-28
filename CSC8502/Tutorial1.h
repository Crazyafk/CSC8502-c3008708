/******************************************************************************
This file is part of the Newcastle OpenGL Tutorial Series
Author:Rich Davison
Contact:richgdavison@gmail.com
License: MIT (see LICENSE file at the top of the source tree)
*//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "TutorialRenderer.h"

namespace NCL::CSC8502 {
	using namespace Rendering;
	class Tutorial1 : public TutorialRenderer {
	public:
		Tutorial1();
		~Tutorial1() = default;

		void RenderFrame() override;

	protected:
		UniqueOGLShader shader;
	};
}