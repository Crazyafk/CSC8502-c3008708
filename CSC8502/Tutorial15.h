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
	class Tutorial15 : public TutorialRenderer {
	public:
		Tutorial15();
		~Tutorial15();

		void RenderFrame() override;

	protected:
		void FillGBufferPass();
		void LightRenderPass();
		void DisplayPass();

		//StructuredOGLBuffer<Light> lights;

		GLuint	gBufferFBO;
		GLuint	lightingFBO;

		GLuint	sceneAlbedoTex;
		GLuint	sceneNormalsTex;
		GLuint	sceneDepthTex;

		GLuint	lightDiffuseTex;
		GLuint	lightSpecularTex;

		UniqueOGLShader gBufferShader;
		UniqueOGLShader lightingShader;
		UniqueOGLShader displayShader;

		UniqueOGLMesh		heightmap;
		UniqueOGLTexture	albedoTex;
		UniqueOGLTexture	bumpTex;
	};
}