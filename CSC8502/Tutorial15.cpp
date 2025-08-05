/******************************************************************************
This file is part of the Newcastle OpenGL Tutorial Series
Author:Rich Davison
Contact:richgdavison@gmail.com
License: MIT (see LICENSE file at the top of the source tree)
*//////////////////////////////////////////////////////////////////////////////
#include "Tutorial15.h"

using namespace NCL;
using namespace Rendering;
using namespace CSC8502;

TUTORIAL_ENTRY(Tutorial15)

const int LIGHT_COUNT = 64;

Tutorial15::Tutorial15() : TutorialRenderer(){
	sceneAlbedoTex = CreateTexture(windowSize.x, windowSize.y, GL_RGBA8);
	sceneNormalsTex = CreateTexture(windowSize.x, windowSize.y, GL_RGBA8);
	sceneDepthTex = CreateTexture(windowSize.x, windowSize.y, GL_DEPTH_COMPONENT24);

	lightDiffuseTex = CreateTexture(windowSize.x, windowSize.y, GL_RGBA16F);
	lightSpecularTex = CreateTexture(windowSize.x, windowSize.y, GL_RGBA16F);

	gBufferShader = std::make_unique<OGLShader>("GBufferFill.vert", "GBufferFill.frag");
	lightingShader = std::make_unique<OGLShader>("DeferredLighting.vert", "DeferredLighting.frag");
	displayShader = std::make_unique<OGLShader>("DeferredDisplay.vert", "DeferredDisplay.frag");

	//for (int i = 0; i < LIGHT_COUNT; ++i) {
	//	Light& l = lights.elements[i];
	//	l.position.x = Maths::RandomValue(-1024, 1024);
	//	l.position.y = 10.0f + Maths::RandomValue(0, 32);
	//	l.position.z = Maths::RandomValue(-1024, 1024);

	//	l.intensity.x = Maths::RandomValue(100.0f, 1000.0f);
	//	l.intensity.y = Maths::RandomValue(100.0f, 1000.0f);
	//	l.intensity.z = Maths::RandomValue(100.0f, 1000.0f);

	//	l.radius = Maths::RandomValue(100.0f, 250.0f);
	//}
	//lights.GPUSync();

	glGenFramebuffers(1, &gBufferFBO);
	glGenFramebuffers(1, &lightingFBO);

	GLenum buffers[2] = {
		GL_COLOR_ATTACHMENT0 ,
		GL_COLOR_ATTACHMENT1
	};

	glBindFramebuffer(GL_FRAMEBUFFER, gBufferFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sceneAlbedoTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, sceneNormalsTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, sceneDepthTex, 0);
	glDrawBuffers(2, buffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, lightingFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, lightDiffuseTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, lightSpecularTex, 0);
	glDrawBuffers(2, buffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		return;
	}

	heightmap = std::make_unique <OGLMesh>();
	//Heightmap::CreateHeightmap("noise.png", *heightmap, { 1024,32.0f,1024 }, { 32, 32 });
	CalculateNormalsTangents(*heightmap);
	heightmap->UploadToGPU(this);

	albedoTex = OGLTexture::TextureFromFile("Barren Reds.JPG");
	bumpTex = OGLTexture::TextureFromFile("Barren RedsDOT3.JPG");

	defaultCamera->SetPosition({ 0, 64, 0 }).SetPitch(-40.0f);
}

Tutorial15::~Tutorial15() {
	glDeleteFramebuffers(1, &gBufferFBO);
	glDeleteFramebuffers(1, &lightingFBO);

	glDeleteTextures(1, &sceneAlbedoTex);
	glDeleteTextures(1, &sceneNormalsTex);
	glDeleteTextures(1, &sceneDepthTex);

	glDeleteTextures(1, &lightDiffuseTex);
	glDeleteTextures(1, &lightSpecularTex);
}

void Tutorial15::RenderFrame() {
	FillGBufferPass();
	LightRenderPass();
	DisplayPass();
}

void Tutorial15::FillGBufferPass() {
	glBindFramebuffer(GL_FRAMEBUFFER, gBufferFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	BindShader(*gBufferShader);
	SetCameraUniforms(*defaultCamera, 0);
	BindTextureToPipeline(albedoTex->GetObjectID(), "albedoTex", 0);
	BindTextureToPipeline(bumpTex->GetObjectID(), "bumpTex", 1);
	SetUniform("modelMatrix", Matrix4());
	BindMesh(*heightmap);
	DrawBoundMesh();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Tutorial15::LightRenderPass() {
	glBindFramebuffer(GL_FRAMEBUFFER, lightingFBO);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_BLEND);
	glCullFace(GL_FRONT);

	BindShader(*lightingShader);
	//BindBufferAsSSBO(lights, 0);
	SetCameraUniforms(*defaultCamera, 0);
	BindTextureToPipeline(sceneDepthTex, "depthTex", 0);
	BindTextureToPipeline(sceneNormalsTex, "normTex", 1);

	float aspect = Window::GetWindow()->GetScreenAspect();
	Matrix4 viewMatrix = defaultCamera->BuildViewMatrix();
	Matrix4 projMatrix = defaultCamera->BuildProjectionMatrix(aspect);

	Matrix4 inverseMatrix = Matrix::Inverse(projMatrix * viewMatrix);


	Vector2 pixelSize = { 1.0f / (float)windowSize.x, 1.0f / (float)windowSize.y };
	SetUniform("inverseProjView", inverseMatrix);
	SetUniform("pixelSize", pixelSize);

	BindMesh(*sphereMesh);
	DrawBoundMesh(0, LIGHT_COUNT);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_BLEND);
	glCullFace(GL_BACK);
}

void Tutorial15::DisplayPass() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);

	BindShader(*displayShader);
	BindTextureToPipeline(sceneAlbedoTex, "albedoTex", 0);
	BindTextureToPipeline(lightDiffuseTex, "diffuseLight", 1);
	BindTextureToPipeline(lightSpecularTex, "specularLight", 2);

	BindMesh(*quadMesh);
	DrawBoundMesh();
}