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
		void BuildNodeLists(SharedNode from);
		void SortNodeLists();
		void ClearNodeLists();
		void DrawNodes();
		void DrawNode(SharedNode node);

		OGLShader shader;
		SharedScene scene;
		StructuredOGLBuffer<FrameConstants> frameConstants;
		std::vector<SharedNode> opaqueNodeList;
	};
}