/******************************************************************************
This file is part of the Newcastle OpenGL Tutorial Series
Author:Rich Davison
Contact:richgdavison@gmail.com
License: MIT (see LICENSE file at the top of the source tree)
*//////////////////////////////////////////////////////////////////////////////
#include "TestProgram.h"

using namespace NCL;
using namespace Rendering;
using namespace CSC8502;

TUTORIAL_ENTRY(TestProgram)

TestProgram::TestProgram() : TutorialRenderer(),
	frameConstants(1)
{

	GLTFLoader::Load("../GLTF/CesiumMan/CesiumMan.gltf", animatedScene);
	GLTFLoader::Load("../GLTF/DamagedHelmet/DamagedHelmet.gltf", staticScene);

	for(const auto& m : animatedScene.meshes) {
		m->UploadToGPU(this);
	}

	for (const auto& m : staticScene.meshes) {
		m->UploadToGPU(this);
	}

	skeletonShader = std::make_unique<OGLShader>("SkinningVertex.glsl", "TexturedFragment.glsl");

	staticShader = std::make_unique<OGLShader>("staticVertex.glsl", "TexturedFragment.glsl");
}

void TestProgram::RenderFrame() {
	BindBufferAsUBO(frameConstants, 0); //0 Matches the binding hard-coded binding in staticVertex.glsl

	frameConstants.elements[0].viewMatrix	= defaultCamera->BuildViewMatrix();
	frameConstants.elements[0].projMatrix	= defaultCamera->BuildProjectionMatrix();

	frameConstants.elements[0].frameTime	= frameTime;
	frameConstants.elements[0].totalTime	= totalRunTime;

	frameConstants.GPUSync();

	DrawStaticScene();
	DrawAnimatedScene();
}

void TestProgram::DrawStaticScene() {
	OGLMesh*		mesh = (OGLMesh*)staticScene.meshes[0].get();
	//Mesh material n relates to mesh n
	GLTFMeshMaterials	material = staticScene.meshMaterials[0];

	BindShader(*staticShader);

	glUniform1i(glGetUniformLocation(staticShader->GetProgramID(), "diffuseTex"), 0);

	Matrix4 modelMatrix = Matrix::Translation(Vector3{ -1,1.5,0 })
		* Matrix::Rotation(180.0f, Vector3(0, 1, 0))
		* Matrix::Rotation(90.0f, Vector3(1, 0, 0))
		* Matrix::Scale(Vector3{ 0.5f,0.5f,0.5f });

	SetUniform("modelMatrix", modelMatrix);

	glActiveTexture(GL_TEXTURE0);
	BindMesh(*mesh);
	for (int i = 0; i < mesh->GetSubMeshCount(); ++i) {
		//Material layer n relates to submesh n
		GLTFMaterial& layer = staticScene.materials[material.layers[i]];

		OGLTexture* albedoTex = (OGLTexture*)staticScene.textures[layer.albedo].get();

		glBindTexture(GL_TEXTURE_2D, albedoTex->GetObjectID());
		DrawBoundMesh(i);
	}
}


void TestProgram::DrawAnimatedScene() {
	OGLMesh* mesh = (OGLMesh*)animatedScene.meshes[0].get();
	GLTFMeshMaterials	material = animatedScene.meshMaterials[0];

	//The loader has a limitation in that animations are stored to 
	//work just for the first animated mesh in the file. Not a problem if it's just 1!
	SharedMeshAnim	anim = animatedScene.animations[0];

	skeleton.resize(anim->GetJointCount());

	frameTime -= frameTime;

	if (frameTime <= 0) {
		currentFrame++;

		frameTime += 1.0f / anim->GetFrameRate();
		currentFrame = currentFrame % anim->GetFrameCount();

		const std::vector<Matrix4> inverseBindPose = mesh->GetInverseBindPose();

		const Matrix4* joints = anim->GetWorldMatrices(currentFrame);

		for (int i = 0; i < skeleton.size(); ++i) {
			skeleton[i] = joints[i] * inverseBindPose[i];
		}
	}

	BindShader(*skeletonShader);
	glUniform1i(glGetUniformLocation(skeletonShader->GetProgramID(), "diffuseTex"), 0);

	Matrix4 modelMatrix = Matrix::Translation(Vector3{ 0,0,0 })
		* Matrix::Rotation(90.0f, Vector3(0, 1, 0))
		* Matrix::Rotation(270.0f, Vector3(1, 0, 0))
		* Matrix::Scale(Vector3{ 1,1,1 });

	//std::vector<Matrix4> frameMatrices;
	//frameMatrices.resize(mesh->GetJointCount());

	//const Matrix4* invBindPose = mesh->GetInverseBindPose();
	//const Matrix4* frameData = anim->GetJointData(currentFrame);

	//for (unsigned int i = 0; i < mesh->GetJointCount(); ++i) {
	//	frameMatrices[i] = (frameData[i] * invBindPose[i]);
	//}

	int j = glGetUniformLocation(skeletonShader->GetProgramID(), "joints");
	glUniformMatrix4fv(j, skeleton.size(), false,
		(float*)skeleton.data());

	glActiveTexture(GL_TEXTURE0);
	BindMesh(*mesh);
	for (int i = 0; i < mesh->GetSubMeshCount(); ++i) {
		//Material layer n relates to submesh n
		GLTFMaterial& layer = staticScene.materials[material.layers[i]];

		OGLTexture* albedoTex = (OGLTexture*)staticScene.textures[layer.albedo].get();

		glBindTexture(GL_TEXTURE_2D, albedoTex->GetObjectID());
		DrawBoundMesh(i);
	}
}
