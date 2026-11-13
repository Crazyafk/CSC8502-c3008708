#include "SceneNode.h"
#include "Scene.h"

using namespace Project;

SceneNode::SceneNode(std::string _name) : GLTFNode() {
	name = _name;
}

SceneNode::SceneNode(const GLTFNode& base) : GLTFNode() {
	name = base.name;
	nodeID = base.nodeID;
	meshID = base.meshID;
	localMatrix = base.localMatrix;
	worldMatrix = base.worldMatrix;
	parent = base.parent;
	children = base.children;
}

void SceneNode::Update(float frameTime) {
	if (parent_p) {
		worldMatrix = parent_p->worldMatrix * localMatrix;
	}
	else {			//Root Node!
		worldMatrix = localMatrix;
	}
	for (SharedNode child : children_v){
		child->Update(frameTime);
	}
}

void SceneNode::AddChild(std::shared_ptr<SceneNode> child, SharedScene inScene) {
	child->parent = nodeID;
	child->parent_p = inScene->GetNode(nodeID);

	children.push_back(child->nodeID);
	children_v.push_back(child);
}

void SceneNode::FindParent(SharedScene inScene) {
	if (parent_p) return;
	if (parent != -1) {
		parent_p = inScene->GetNode(parent);
	}
}

void SceneNode::FindChildren(SharedScene inScene) {
	if (children.empty()) return;
	if (!children_v.empty()) return;
	for (int32_t child : children) {
		children_v.push_back(inScene->GetNode(child));
	}
}

bool SceneNode::FindMeshNode(SharedNode& out) {
	if (HasMesh()) return true;

	for (SharedNode child : children_v) {
		if (child->FindMeshNode(out)) {
			out = out ? out : child;
			return true;
		}
	}
	out = nullptr;
	return false;
}

SharedMesh SceneNode::GetMesh(SharedScene inScene) { return HasMesh() ? inScene->GetMesh(meshID) : nullptr; }
GLTFMeshMaterials SceneNode::GetMeshMaterials(SharedScene inScene) { return inScene->GetMeshMaterials(meshID); }
SharedMeshAnim SceneNode::GetAnimation(SharedScene inScene) { return isAnimated() ? inScene->GetAnimation(animation_id) : nullptr; }

/*
* Integrates a GLTF Scene into the given parent scene and returns pointer to the node at the root of the GLTF Scene.
* Note: Makes the given GLTF Scene invalid.
*/
std::shared_ptr<SceneNode> SceneNode::FromGLTFScene(GLTFScene& _object, SharedScene parentScene, SharedNode setParent) {
	/*
	* Implementation Notes
	* - Friended by Scene (needed to add vector data)
	* Steps:
	* 1. Collect Data (vector offsets)
	* 2. Shift ID references
	* 3. Add Vector Data
	* 4. Add SceneNode Objects
	*/

	if (!setParent) setParent = parentScene->GetRoot();

	//Step 1
	int32_t firstAnim = parentScene->GetAnimationsCount();
	int32_t firstMat = parentScene->GetMeshMaterialsCount();
	int32_t firstMatLayer = parentScene->GetMaterialsCount();
	int32_t firstMesh = parentScene->GetMeshesCount();
	int32_t firstNode = parentScene->GetNodesCount() + 1; //we never added the root node to the underlying GLTF node array
	int32_t firstTex = parentScene->GetTexturesCount();

	//Step 2
	for (GLTFNode& node : _object.sceneNodes) {
		node.nodeID += firstNode;
		node.meshID += firstMesh;
		if (node.parent != -1) node.parent += firstNode;
		for (int32_t& child : node.children) {
			child += firstNode;
		}
	}
	for (auto const& pair : _object.skinningData) { //Assuming the key of skinning data is the node id?
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
		std::shared_ptr<SceneNode> sNode = std::make_shared<SceneNode>(node);
		parentScene->allNodes.push_back(sNode);
	}
	for (auto &node : parentScene->allNodes) {
		node->FindParent(parentScene);
		node->FindChildren(parentScene);
	}

	int32_t root_ID = _object.topLevelNodes[0];
	SharedNode root = parentScene->GetNode(root_ID);
	setParent->AddChild(root, parentScene);

	return root;
}