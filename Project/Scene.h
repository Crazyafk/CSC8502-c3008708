#pragma once
#include "..\GLTFLoader\GLTFLoader.h"
#include "SceneNode.h"

namespace Project {
	using namespace NCL::Rendering;

	class Scene : protected GLTFScene {
	public:
		Scene() = default;
		std::shared_ptr<SceneNode> GetNode(int32_t id) { return allNodes[id]; }

		size_t GetMeshesCount() { return meshes.size(); }
		size_t GetAnimationsCount() { return animations.size(); }
		size_t GetTexturesCount() { return textures.size(); }
		size_t GetMeshMaterialsCount() { return meshMaterials.size(); }
		size_t GetMaterialsCount() { return materials.size(); }
		size_t GetNodesCount() { return sceneNodes.size(); }

		friend std::shared_ptr<SceneNode> SceneNode::FromGLTFScene(GLTFScene& _object, std::shared_ptr<Scene> parentScene);
	protected:
		std::vector<std::shared_ptr<SceneNode>> allNodes;
	};
}