#include "SceneRenderer.h"
#include "SceneNode.h"
#include "Scene.h"
#include "../MatrixUtils.h"
#include "SceneUtils.h"
#include "../../OpenGLRendering/OGLTexture.h"
#include <algorithm>

using namespace NCL;
using namespace Rendering;
using namespace Project;

typedef std::shared_ptr<Scene> SharedScene;
typedef std::shared_ptr<SceneNode> SharedNode;

SceneRenderer::SceneRenderer() : Renderer(),
frameConstants(1)
{
	scene = std::make_shared<Scene>();

	defaultCamera->SetPosition({ -0.5f, 1.5, -3.0f })
		.SetPitch(0.0f)
		.SetYaw(180.0f);
}

void SceneRenderer::RenderFrame() {
	UpdateUBO();
	RenderSkybox();
	scene->Update(frameTime);

	UploadLightUniforms();

	BuildNodeLists(scene->GetRoot());
	SortNodeLists();

	DrawShadowScene(*shadowingLight);
	glClear(GL_DEPTH_BUFFER_BIT); //shouldn't be needed. mysteryyyyy :3

	BindShader(defaultShader);
	DrawNodes();

	ClearNodeLists();
}

void SceneRenderer::BeforeFirstFrame() {
	BuildSkybox();
	std::cout << "Populating Scene!";
	PopulateScene();
}

void SceneRenderer::BuildSkybox() {
	std::vector<std::string> paths = GetSkyboxTexturePaths();
	if (paths.empty()) return;

	skybox.mesh = CreateSkyboxQuad();
	skybox.tex = OGLTexture::LoadCubemap(
		paths[0], paths[1], paths[2], paths[3], paths[4], paths[5]
	);
	skybox.initialised = true;
}

void SceneRenderer::RenderSkybox() {
	if (!skybox.initialised) return;
	BindShader(skybox.shader);
	glDepthMask(GL_FALSE);

	SetUniform("viewNoTranslation", NoTranslation(frameConstants.elements[0].viewMatrix));

	BindMesh(*static_cast<OGLMesh*>(skybox.mesh.get()));
	SetUniform("skybox", 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.tex->GetObjectID());
	DrawBoundMesh();

	glDepthMask(GL_TRUE);
}

void SceneRenderer::UpdateUBO() {
	BindBufferAsUBO(frameConstants, 0); //0 Matches the binding hard-coded binding in staticVertex.glsl

	frameConstants.elements[0].viewMatrix = defaultCamera->BuildViewMatrix();
	frameConstants.elements[0].projMatrix = defaultCamera->BuildProjectionMatrix();
	frameConstants.elements[0].viewProjMatrix = defaultCamera->BuildProjectionMatrix() * defaultCamera->BuildViewMatrix();
	frameConstants.elements[0].frameTime = frameTime;
	frameConstants.elements[0].totalTime = totalRunTime;
	frameConstants.elements[0].cameraPos = defaultCamera->GetPosition();

	frameConstants.GPUSync();
}

void SceneRenderer::ShadowDrawCallback(const DirectionLight& light) {
	DrawOpaqueNodes();
}

void SceneRenderer::BuildNodeLists(SharedNode from) {
	// TODO: frustum checks

	if (from->HasMesh()) {
		Vector3 dir = GetMatrixPosition(from->GetWorldMatrix()) -
			defaultCamera->GetPosition();
		from->SetCameraDistance(Vector::Dot(dir, dir));

		if (from->isTransparent()) {
			transparentNodeList.push_back(from);
		}
		else {
			opaqueNodeList.push_back(from);
		}
	}

	for (SharedNode child : from->GetChildren()) {
		BuildNodeLists(child);
	}
}

void SceneRenderer::SortNodeLists() {
	std::sort(opaqueNodeList.begin(),
		opaqueNodeList.end(),
		SceneNode::CompareByCameraDistance);
	std::sort(transparentNodeList.rbegin(),
		transparentNodeList.rend(),
		SceneNode::CompareByCameraDistance);
}

void SceneRenderer::ClearNodeLists() {
	opaqueNodeList.clear();
	transparentNodeList.clear();
}

void SceneRenderer::DrawNodes() {
	DrawOpaqueNodes();
	DrawTransparentNodes();
}

void SceneRenderer::DrawOpaqueNodes() {
	glDepthMask(GL_TRUE);
	for (SharedNode node : opaqueNodeList) {
		DrawNode(node);
	}
}
void SceneRenderer::DrawTransparentNodes() {
	glDepthMask(GL_FALSE);
	for (SharedNode node : transparentNodeList) {
		DrawNode(node);
	}
}

void SceneRenderer::DrawNode(SharedNode node) {
	SetUniform("debug_nodeID", (int)node->GetID());
	SetUniform("debug_distanceFromCamera", node->GetCameraDistance());

	SetUniform("isAnimated", node->isAnimated() ? 1 : 0);
	if (node->isAnimated()) AnimateNode(node);
	
	SharedMesh mesh = (SharedMesh)node->GetMesh(scene);
	GLTFMeshMaterials	material = node->GetMeshMaterials(scene);
	
	SetUniform("modelMatrix", node->GetWorldMatrix());
	
	SetUniform("diffuseTex", 0);
	glActiveTexture(GL_TEXTURE0);
	BindMesh(*static_cast<OGLMesh*>(mesh.get()));
	
	for (int i = 0; i < mesh->GetSubMeshCount(); ++i) {
		//Material layer n relates to submesh n
		GLTFMaterial& layer = scene->GetMaterial(material.layers[i]);

		bool hasTexture = layer.albedo != -1;
		SetUniform("hasTexture", hasTexture ? 1 : 0);

		if (hasTexture) {
			SharedOGLTexture albedoTex = static_pointer_cast<OGLTexture>(scene->GetTexture(layer.albedo));
			glBindTexture(GL_TEXTURE_2D, albedoTex->GetObjectID());
		}
		else {
			SetUniform("albedoColour", layer.albedoColour);
		}
		
		DrawBoundMesh(i);
	}
}

void SceneRenderer::AnimateNode(SharedNode node) {
	SharedSkeletalAnimator animator = node->GetAnimator();
	animator->Update(frameTime);
	SetUniform("joints", *(animator->skeleton.data()), animator->skeleton.size());
}