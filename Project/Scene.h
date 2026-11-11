#pragma once
#include "..\GLTFLoader\GLTFLoader.h"
#include "SceneNode.h"

namespace Project {
	using namespace NCL::Rendering;
	typedef std::shared_ptr<Scene> SharedScene;
	typedef std::shared_ptr<SceneNode> SharedNode;

	class Scene : protected GLTFScene {
	public:
		Scene();
		SharedNode GetNode(int32_t id) { return allNodes[id]; }
		SharedNode GetRoot() { return root; }
		void AddNode(SharedNode node);

		void UploadMeshes(RendererBase* renderer);

		size_t GetMeshesCount() { return meshes.size(); }
		size_t GetAnimationsCount() { return animations.size(); }
		size_t GetTexturesCount() { return textures.size(); }
		size_t GetMeshMaterialsCount() { return meshMaterials.size(); }
		size_t GetMaterialsCount() { return materials.size(); }
		size_t GetNodesCount() { return sceneNodes.size(); }

		SharedMesh GetMesh(int32_t id) { return meshes[id]; }

		friend SharedNode SceneNode::FromGLTFScene(GLTFScene& _object, SharedScene parentScene, SharedNode setParent);
	protected:
		std::vector<SharedNode> allNodes;
		SharedNode root;
	};
}