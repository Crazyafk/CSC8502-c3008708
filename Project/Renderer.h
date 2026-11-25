/*
* Adapted from CSC8502/TutorialRenderer
*/
#pragma once
#include "OGLRenderer.h"
#include "GLTFLoader.h"
#include "OGLBuffer.h"
#include "OGLMesh.h"
#include "OGLShader.h"
#include "OGLTexture.h"

#include "../NCLCoreClasses/Camera.h"
#include "../NCLCoreClasses/Window.h"

#include "FrameBuffer.h"

#include "Lighting/Light.h"
#include "Lighting/PointLight.h"
#include "Lighting/DirectionLight.h"

namespace NCL::Rendering {
#include "./Shaders/glslInterop.h"
#include "./Shaders/TestStructs.glslh"
#include "./Shaders/LightStructs.h"
}

namespace Project {
	using namespace NCL;
	using namespace NCL::Rendering;

	class Renderer : public OGLRenderer {
	public:
		Renderer();
		~Renderer();

		virtual void Update(float dt);

		void WindowEventHandler(NCL::WindowEvent e, uint32_t w, uint32_t h);

		Camera& GetDefaultCamera() { return *defaultCamera; }
	protected:
		virtual void BehaviourUpdate() {};
		virtual void ShadowDrawCallback(const DirectionLight& light) {};
		virtual void PostProcess() { finalTex = sceneTex; }; //do nothing

		UniqueOGLMesh CreateTriangle();
		UniqueOGLMesh CreateQuad();
		UniqueOGLMesh CreateSkyboxCube();
		UniqueOGLMesh CreateScreenQuad();

		void DrawShadowScene(const DirectionLight& light, Matrix4& outShadowMatrix);
		void UploadShadowTex();
		void PreSceneDraw();
		void PresentToScreen();

		GLuint	CreateTexture(uint32_t width, uint32_t height, GLint internalformat, bool allocMips = false);

		void BindTextureToPipeline(GLuint texID, const std::string& name, GLuint texUnit, GLuint texType = GL_TEXTURE_2D);

		bool GetUniformLocation(const std::string& name, GLint& locationOut);
		void SetUniform(const std::string& name, const Matrix4& mat, int arrayLength = 1);
		void SetUniform(const std::string& name, const Vector4& v);
		void SetUniform(const std::string& name, const Vector3& v);
		void SetUniform(const std::string& name, const Vector2& v);
		void SetUniform(const std::string& name, int i);
		void SetUniform(const std::string& name, float f);

		void BindBufferAsUBO(const OGLBuffer& b, GLuint slotID);
		void BindBufferAsSSBO(const OGLBuffer& b, GLuint slotID);

		void SetCameraUniforms(const Camera& c, GLuint slot);
		void UploadLightUniforms();

		void CalculateNormals(Mesh& m) const;
		void CalculateNormalsTangents(Mesh& m) const;

		UniqueOGLMesh screenQuad;

		OGLShader defaultShader = OGLShader("UniversalVertex.glsl", "UniversalFragment.glsl");
		OGLShader shadowShader = OGLShader("ShadowVertex.glsl", "ShadowFragment.glsl");
		OGLShader screenShader = OGLShader("ScreenVertex.glsl", "TexturedFragment.glsl");

		SharedOGLTexture shadowTex = std::make_shared<OGLTexture>();
		SharedFrameBuffer shadowFBO;
		StructuredOGLBuffer<NCL::Rendering::FrameConstants> shadowUBO; 
		bool drawingShadows = false;

		SharedOGLTexture sceneTex = std::make_shared<OGLTexture>();
		SharedOGLTexture sceneDepth = std::make_shared<OGLTexture>();
		SharedFrameBuffer sceneFBO;
		SharedOGLTexture finalTex;

		Camera* defaultCamera;

		Controller* controller;

		std::vector<SharedLight> lights;
		StructuredOGLBuffer<NCL::Rendering::LightData> lightBuffer = StructuredOGLBuffer<NCL::Rendering::LightData>(16); //matches hardcoded max_lights in UniversalFragment.glsl;

		GLTFScene gltf;

		float totalRunTime = 0.0f;
		float frameTime = 0.0f;
	};
}