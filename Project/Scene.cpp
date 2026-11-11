#include "Scene.h"
#include "SceneNode.h"

using namespace Project;

Scene::Scene() {
	root = std::make_shared<SceneNode>(std::string("Scene Root"));
	AddNode(root);
}

// NOTE: ASSUMES THE NODE IS EMPTY!
void Scene::AddNode(SharedNode node) {
	allNodes.push_back(node);
}