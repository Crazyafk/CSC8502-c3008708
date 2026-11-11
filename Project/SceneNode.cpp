#include "SceneNode.h"
#include "Scene.h"

using namespace Project;

SceneNode::SceneNode(std::string _name, std::shared_ptr<Scene> _scene) : GLTFNode() {
	scene = _scene;
	name = _name;
}

SceneNode::SceneNode(const GLTFNode& base, std::shared_ptr<Scene> _scene) : GLTFNode() {
	scene = _scene;
	name = base.name;
	nodeID = base.nodeID;
	meshID = base.meshID;
	localMatrix = base.localMatrix;
	worldMatrix = base.worldMatrix;
	parent = base.parent;
	children = base.children;
}

void SceneNode::AddChild(std::shared_ptr<SceneNode> child) {
	child->parent = nodeID;
	child->parent_p = std::shared_ptr<SceneNode>(this);

	children.push_back(child->nodeID);
	children_v.push_back(child);
}

void SceneNode::FindParent() {
	if (parent != -1) {
		parent_p = scene->GetNode(parent);
	}
}

void SceneNode::FindChildren() {
	for (int32_t child : children) {
		children_v.push_back(scene->GetNode(child));
	}
}

/*
* Integrates a GLTF Scene into the given parent scene and returns pointer to the node at the root of the GLTF Scene.
* Note: Makes the given GLTF Scene invalid.
*/
std::shared_ptr<SceneNode> SceneNode::FromGLTFScene(GLTFScene& _object, std::shared_ptr<Scene> parentScene) {
	/*
	* Implementation Notes
	* - Friended by Scene (needed to add vector data)
	* Steps:
	* 1. Collect Data (vector offsets)
	* 2. Shift ID references
	* 3. Add Vector Data
	* 4. Add SceneNode Objects
	*/

	//Step 1
	int32_t firstAnim = parentScene->GetAnimationsCount();
	int32_t firstMat = parentScene->GetMeshMaterialsCount();
	int32_t firstMatLayer = parentScene->GetMaterialsCount();
	int32_t firstMesh = parentScene->GetMeshesCount();
	int32_t firstNode = parentScene->GetNodesCount();
	int32_t firstTex = parentScene->GetTexturesCount();

	//Step 2
	for (GLTFNode& node : _object.sceneNodes) {
		node.nodeID += firstNode;
		node.meshID += firstMesh;
		node.parent += firstNode;
		for (int32_t child : node.children) {
			child += firstNode;
		}
	}
	for (auto const& pair : _object.skinningData) { //Assuming the key of skinning data is the node id? please leave more comments rich ;_;
		parentScene->skinningData[pair.first + firstNode] = pair.second;
	}
	for (auto& mat : _object.meshMaterials) {
		for (auto layer : mat.layers) {
			layer += firstMatLayer;
		}
	}
	for (auto& node : _object.topLevelNodes) {
		node += firstNode;
	}
	//Does anything else need to be offset? MAYBE

	//Step 3
	for (auto mesh : _object.meshes) {
		parentScene->meshes.push_back(mesh);
	}
	for (auto anim : _object.animations) {
		parentScene->animations.push_back(anim);
	}
	for (auto tex : _object.textures) {
		parentScene->textures.push_back(tex);
	}

	for (auto meshMaterial : _object.meshMaterials) {
		parentScene->meshMaterials.push_back(meshMaterial);
	}
	for (auto material : _object.materials) {
		parentScene->materials.push_back(material);
	}
	for (auto materialName : _object.materialNames) {
		parentScene->materialNames.push_back(materialName);
	}
	//Skinning Data copied earlier

	for (GLTFNode node : _object.sceneNodes) {
		parentScene->sceneNodes.push_back(node);
	}
	//NOT COPYING TOP-LEVEL NODES SINCE IT WOULD CONTRADICT THE WHOLE POINT OF THIS FUNCTION

	//Step 4
	for (const GLTFNode &node : _object.sceneNodes) {
		std::shared_ptr<SceneNode> sNode = std::make_shared<SceneNode>(node, parentScene);
		parentScene->allNodes.push_back(sNode);
	}
	for (auto &node : parentScene->allNodes) {
		node->FindParent();
		node->FindChildren();
	}

	int32_t root_ID = _object.topLevelNodes[0];
	return parentScene->GetNode(root_ID);
}