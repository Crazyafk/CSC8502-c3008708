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
		UniqueOGLMesh CreateTriangle();
		UniqueOGLMesh CreateQuad();

		GLuint	CreateTexture(uint32_t width, uint32_t height, GLint internalformat, bool allocMips = false);

		void BindTextureToPipeline(GLuint texID, const std::string& name, GLuint texUnit, GLuint texType = GL_TEXTURE_2D);

		void SetUniform(const std::string& name, const Matrix4& mat);
		void SetUniform(const std::string& name, const Vector2& mat);
		void SetUniform(const std::string& name, int i);

		void BindBufferAsUBO(const OGLBuffer& b, GLuint slotID);
		void BindBufferAsSSBO(const OGLBuffer& b, GLuint slotID);

		void SetCameraUniforms(const Camera& c, GLuint slot);
		//void SetLightUniforms(const Light& l, GLuint slot);

		void CalculateNormals(Mesh& m) const;
		void CalculateNormalsTangents(Mesh& m) const;

		UniqueOGLMesh triMesh;			
		UniqueOGLMesh quadMesh;			
		SharedOGLMesh sphereMesh;

		Camera* defaultCamera;

		Controller* controller;

		GLTFScene gltf;

		float totalRunTime = 0.0f;
		float frameTime = 0.0f;
	};
}