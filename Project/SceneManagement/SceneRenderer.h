#pragma once
#include "../Renderer.h"
#include "../Lighting/PointLight.h"
#include "Scene.h"

#include "./Shaders/glslinterop.h"

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
		void ShadowDrawCallback(const DirectionLight& light) override;
		void BuildNodeLists(SharedNode from);
		void SortNodeLists();
		void ClearNodeLists();
		void DrawNodes();
		void DrawOpaqueNodes();
		void DrawTransparentNodes();
		void DrawNode(SharedNode node);
		void AnimateNode(SharedNode node);

		SharedScene scene;
		std::shared_ptr<DirectionLight> shadowingLight;
		StructuredOGLBuffer<FrameConstants> frameConstants;
		std::vector<SharedNode> opaqueNodeList;
		std::vector<SharedNode> transparentNodeList;
		SkyBox skybox{ nullptr, nullptr, OGLShader("SkyboxVertex.glsl", "SkyboxFragment.glsl"), false};
	};
}