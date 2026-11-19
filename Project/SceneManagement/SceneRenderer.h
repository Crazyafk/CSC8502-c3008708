#pragma once
#include "../Renderer.h"
#include "../Lighting/PointLight.h"
#include "Scene.h"

#include "./Shaders/glslinterop.h"
#include "./Shaders/TestStructs.glslh"

namespace Project {
	using namespace NCL::Rendering;

	struct SkyBox {
		SharedMesh mesh;
		UniqueOGLTexture tex;
		OGLShader shader;
		bool initialised = false;
	};

	class SceneRenderer : public Renderer {
	public:
		SceneRenderer();
		~SceneRenderer() = default;

		void RenderFrame() override;
	protected:
		virtual std::vector<std::string> GetSkyboxTexturePaths() { return{}; };
		void BeforeFirstFrame() override;
		virtual void PopulateScene() {};
		void BuildSkybox();
		void RenderSkybox();
		void UpdateUBO();
		void BuildNodeLists(SharedNode from);
		void SortNodeLists();
		void ClearNodeLists();
		void DrawNodes();
		void DrawNode(SharedNode node);
		void AnimateNode(SharedNode node);

		OGLShader shader;
		SharedScene scene;
		std::unique_ptr<PointLight> light;
		StructuredOGLBuffer<FrameConstants> frameConstants;
		std::vector<SharedNode> opaqueNodeList;
		std::vector<SharedNode> transparentNodeList;
		SkyBox skybox{ nullptr, nullptr, OGLShader("SkyboxVertex.glsl", "SkyboxFragment.glsl"), false};
	};
}