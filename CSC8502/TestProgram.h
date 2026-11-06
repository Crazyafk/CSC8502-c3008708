/******************************************************************************
This file is part of the Newcastle OpenGL Tutorial Series
Author:Rich Davison
Contact:richgdavison@gmail.com
License: MIT (see LICENSE file at the top of the source tree)
*//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "TutorialRenderer.h"

#include "./Shaders/glslinterop.h"
#include "./Shaders/TestStructs.glslh"

namespace NCL::CSC8502 {
	using namespace Rendering;
	class TestProgram : public TutorialRenderer {
	public:
		TestProgram();
		~TestProgram() = default;

		void RenderFrame() override;

	protected:
		void DrawStaticScene();
		void DrawAnimatedScene();

		OGLShader skeletonShader;
		OGLShader staticShader;

		StructuredOGLBuffer< FrameConstants> frameConstants;

		std::vector<Matrix4> skeleton;

		GLTFScene animatedScene;
		GLTFScene staticScene;

		int currentFrame = 0;

		float animTime = 0.0f;
	};
}