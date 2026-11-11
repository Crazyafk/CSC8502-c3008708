/******************************************************************************
Adapted from CSC8052/TestProgram
*//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "Renderer.h"

#include "./Shaders/glslinterop.h"
#include "./Shaders/TestStructs.glslh"

namespace Project {
	using namespace NCL::Rendering;
	class TestProgram : public Renderer {
	public:
		TestProgram();
		~TestProgram() = default;

		void RenderFrame() override;

	protected:
		void DrawStaticScene();
		void DrawAnimatedScene();

		OGLShader shader;

		StructuredOGLBuffer< FrameConstants> frameConstants;

		std::vector<Matrix4> skeleton;

		GLTFScene animatedScene;
		GLTFScene staticScene;

		int currentFrame = 0;

		float animTime = 0.0f;
	};
}