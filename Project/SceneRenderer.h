#pragma once
#include "Renderer.h"
#include "Scene.h"

#include "./Shaders/glslinterop.h"
#include "./Shaders/TestStructs.glslh"

namespace Project {
	using namespace NCL::Rendering;
	class SceneRenderer : public Renderer {
	public:
		SceneRenderer();
		~SceneRenderer() = default;

		void RenderFrame() override;
	protected:
		void PopulateScene();
		void UpdateUBO();

		OGLShader shader;
		SharedScene scene;
		StructuredOGLBuffer<FrameConstants> frameConstants;
	};
}