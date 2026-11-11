#include "Scene.h"
#include "SceneNode.h"

using namespace Project;
using namespace NCL;
using namespace Rendering;

Scene::Scene() {
	root = std::make_shared<SceneNode>(std::string("Scene Root"));
	AddNode(root);
}

// NOTE: ASSUMES THE NODE IS EMPTY!
void Scene::AddNode(SharedNode node) {
	allNodes.push_back(node);
}

void Scene::UploadMeshes(RendererBase* renderer) {
	for (const auto& m : meshes) {
		m->UploadToGPU(renderer);
	}
}