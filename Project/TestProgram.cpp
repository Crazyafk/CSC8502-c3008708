/******************************************************************************
Adapted from CSC8052/TestProgram
*//////////////////////////////////////////////////////////////////////////////
#include "TestProgram.h"
#include "SceneNode.h"
#include "Scene.h"

using namespace NCL;
using namespace Rendering;
using namespace Project;

typedef std::shared_ptr<Scene> SharedScene;
typedef std::shared_ptr<SceneNode> SharedNode;

TestProgram::TestProgram() : Renderer(),
	frameConstants(1),
	skeletonShader("SkinningVertex.glsl", "TexturedFragment.glsl"),
	staticShader("staticVertex.glsl", "TexturedFragment.glsl")
{
	GLTFLoader::Load("../GLTF/CesiumMan/CesiumMan.gltf", animatedScene);
	GLTFLoader::Load("../GLTF/DamagedHelmet/DamagedHelmet.gltf", staticScene);

	SharedScene scene = std::make_shared<Scene>();
	SharedNode cesiumMan = SceneNode::FromGLTFScene(animatedScene, scene);
	SharedNode helmet = SceneNode::FromGLTFScene(staticScene, scene);

	for(const auto& m : animatedScene.meshes) {
		m->UploadToGPU(this);
	}

	for (const auto& m : staticScene.meshes) {
		m->UploadToGPU(this);
	}

	defaultCamera->SetPosition({ -0.5f, 1.5, -3.0f })
		.SetPitch(0.0f)
		.SetYaw(180.0f);
}

void TestProgram::RenderFrame() {
	BindBufferAsUBO(frameConstants, 0); //0 Matches the binding hard-coded binding in staticVertex.glsl

	frameConstants.elements[0].viewMatrix		= defaultCamera->BuildViewMatrix();
	frameConstants.elements[0].projMatrix		= defaultCamera->BuildProjectionMatrix();
	frameConstants.elements[0].viewProjMatrix	= defaultCamera->BuildProjectionMatrix() * defaultCamera->BuildViewMatrix();
	frameConstants.elements[0].frameTime		= frameTime;
	frameConstants.elements[0].totalTime		= totalRunTime;

	frameConstants.GPUSync();

	DrawStaticScene();
	DrawAnimatedScene();
}

void TestProgram::DrawStaticScene() {
	OGLMesh*		mesh = (OGLMesh*)staticScene.meshes[0].get();
	//Mesh material n relates to mesh n
	GLTFMeshMaterials	material = staticScene.meshMaterials[0];

	BindShader(staticShader);

	Matrix4 modelMatrix = Matrix::Translation(Vector3{ -1,1.5,0 })
		* Matrix::Rotation(180.0f, Vector3(0, 1, 0))
		* Matrix::Rotation(90.0f, Vector3(1, 0, 0))
		* Matrix::Scale(Vector3{ 0.5f,0.5f,0.5f });

	SetUniform("modelMatrix", modelMatrix);

	glUniform1i(glGetUniformLocation(staticShader.GetProgramID(), "diffuseTex"), 0);
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

	animTime -= frameTime;

	if (animTime <= 0) {
		currentFrame++;

		animTime += 1.0f / anim->GetFrameRate();
		currentFrame = currentFrame % anim->GetFrameCount();

		const std::vector<Matrix4> inverseBindPose = mesh->GetInverseBindPose();

		std::span<const Maths::Matrix4> joints = anim->GetWorldMatrices(currentFrame);

		for (int i = 0; i < skeleton.size(); ++i) {
			skeleton[i] = joints[i] * inverseBindPose[i];
		}
	}

	BindShader(skeletonShader);

	Matrix4 modelMatrix = Matrix::Translation(Vector3{ 0,0,0 })
		* Matrix::Rotation(90.0f, Vector3(0, 1, 0))
		* Matrix::Rotation(270.0f, Vector3(1, 0, 0))
		* Matrix::Scale(Vector3{ 1,1,1 });

	SetUniform("modelMatrix", modelMatrix);

	int j = glGetUniformLocation(skeletonShader.GetProgramID(), "joints");
	glUniformMatrix4fv(j, skeleton.size(), false,(float*)skeleton.data());

	glUniform1i(glGetUniformLocation(skeletonShader.GetProgramID(), "diffuseTex"), 0);
	glActiveTexture(GL_TEXTURE0);

	BindMesh(*mesh);
	for (int i = 0; i < mesh->GetSubMeshCount(); ++i) {
		//Material layer n relates to submesh n
		GLTFMaterial& layer = animatedScene.materials[material.layers[i]];

		OGLTexture* albedoTex = (OGLTexture*)animatedScene.textures[layer.albedo].get();

		glBindTexture(GL_TEXTURE_2D, albedoTex->GetObjectID());
		DrawBoundMesh(i);
	}
}
