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
		UniqueOGLMesh CreateTriangle();
		UniqueOGLMesh CreateQuad();
		UniqueOGLMesh CreateSkyboxCube();
		UniqueOGLMesh CreateSkyboxQuad();

		void DrawShadowScene(const DirectionLight& light);

		GLuint	CreateTexture(uint32_t width, uint32_t height, GLint internalformat, bool allocMips = false);

		void BindTextureToPipeline(GLuint texID, const std::string& name, GLuint texUnit, GLuint texType = GL_TEXTURE_2D);

		void SetUniform(const std::string& name, const Matrix4& mat, int arrayLength = 1);
		void SetUniform(const std::string& name, const Vector4& mat);
		void SetUniform(const std::string& name, const Vector2& mat);
		void SetUniform(const std::string& name, int i);
		void SetUniform(const std::string& name, float f);

		void BindBufferAsUBO(const OGLBuffer& b, GLuint slotID);
		void BindBufferAsSSBO(const OGLBuffer& b, GLuint slotID);

		void SetCameraUniforms(const Camera& c, GLuint slot);
		void UploadLightUniforms();

		void CalculateNormals(Mesh& m) const;
		void CalculateNormalsTangents(Mesh& m) const;

		UniqueOGLMesh triMesh;			
		UniqueOGLMesh quadMesh;			
		SharedOGLMesh sphereMesh;

		OGLShader defaultShader = OGLShader("UniversalVertex.glsl", "UniversalFragment.glsl");
		OGLShader shadowShader = OGLShader("ShadowVertex.glsl", "ShadowFragment.glsl");

		SharedOGLTexture shadowTex;
		GLuint shadowFBO; //maybe FBO class in future? better understanding of workings and uses needed
		StructuredOGLBuffer<NCL::Rendering::FrameConstants> shadowUBO; 

		Camera* defaultCamera;

		Controller* controller;

		std::vector<SharedLight> lights;
		StructuredOGLBuffer<NCL::Rendering::LightData> lightBuffer = StructuredOGLBuffer<NCL::Rendering::LightData>(16); //matches hardcoded max_lights in UniversalFragment.glsl;

		GLTFScene gltf;

		float totalRunTime = 0.0f;
		float frameTime = 0.0f;
	};
}