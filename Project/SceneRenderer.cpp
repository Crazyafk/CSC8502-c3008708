#include "SceneRenderer.h"
#include "SceneNode.h"
#include "Scene.h"
#include "MatrixUtils.h"
#include <algorithm>

using namespace NCL;
using namespace Rendering;
using namespace Project;

typedef std::shared_ptr<Scene> SharedScene;
typedef std::shared_ptr<SceneNode> SharedNode;

SceneRenderer::SceneRenderer() : Renderer(),
	frameConstants(1),
	shader("UniversalVertex.glsl", "TexturedFragment.glsl")
{
	scene = std::make_shared<Scene>();
	PopulateScene();

	defaultCamera->SetPosition({ -0.5f, 1.5, -3.0f })
		.SetPitch(0.0f)
		.SetYaw(180.0f);
}

void SceneRenderer::RenderFrame() {
	UpdateUBO();
	scene->Update(frameTime);

	BuildNodeLists(scene->GetRoot());
	SortNodeLists();
	BindShader(shader);
	DrawNodes();

	ClearNodeLists();
}

void SceneRenderer::PopulateScene() {
	GLTFScene tempSceneA;
	GLTFLoader::Load("../GLTF/CesiumMan/CesiumMan.gltf", tempSceneA);
	SharedNode cesiumMan = SceneNode::FromGLTFScene(tempSceneA, scene);
	cesiumMan->SetLocalMatrix(Matrix::Translation(Vector3{ 0,0,0 })
		* Matrix::Rotation(90.0f, Vector3(0, 1, 0))
		* Matrix::Rotation(270.0f, Vector3(1, 0, 0))
		* Matrix::Scale(Vector3{ 1,1,1 }));

	GLTFScene tempSceneB;
	GLTFLoader::Load("../GLTF/DamagedHelmet/DamagedHelmet.gltf", tempSceneB);
	SharedNode helmet = SceneNode::FromGLTFScene(tempSceneB, scene);
	helmet->SetLocalMatrix(Matrix::Translation(Vector3{ -1, 1.5, 0 })
		* Matrix::Rotation(180.0f, Vector3(0, 1, 0))
		* Matrix::Rotation(90.0f, Vector3(1, 0, 0))
		* Matrix::Scale(Vector3{ 0.5f,0.5f,0.5f }));
	helmet->SetAnimation(0);

	scene->UploadMeshes(this);
}

void SceneRenderer::UpdateUBO() {
	BindBufferAsUBO(frameConstants, 0); //0 Matches the binding hard-coded binding in staticVertex.glsl

	frameConstants.elements[0].viewMatrix = defaultCamera->BuildViewMatrix();
	frameConstants.elements[0].projMatrix = defaultCamera->BuildProjectionMatrix();
	frameConstants.elements[0].viewProjMatrix = defaultCamera->BuildProjectionMatrix() * defaultCamera->BuildViewMatrix();
	frameConstants.elements[0].frameTime = frameTime;
	frameConstants.elements[0].totalTime = totalRunTime;

	frameConstants.GPUSync();
}

void SceneRenderer::BuildNodeLists(SharedNode from) {
	// TODO: frustum checks

	Vector3 dir = GetMatrixPosition(from->GetWorldMatrix()) - 
		defaultCamera->GetPosition();
	from->SetCameraDistance(Vector::Dot(dir, dir));

	opaqueNodeList.push_back(from);

	for (SharedNode child : from->GetChildren()) {
		BuildNodeLists(child);
	}
}

void SceneRenderer::SortNodeLists() {
	std::sort(opaqueNodeList.begin(),
		opaqueNodeList.end(),
		SceneNode::CompareByCameraDistance);
}

void SceneRenderer::ClearNodeLists() {
	opaqueNodeList.clear();
}

void SceneRenderer::DrawNodes() {
	for (SharedNode node : opaqueNodeList) {
		DrawNode(node);
	}
	// TODO: transparent node support:
	//		Edit other node list funcs too
	//		use rbegin() and rend() in Sort
}

void SceneRenderer::DrawNode(SharedNode node) {

}