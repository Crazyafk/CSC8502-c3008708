#pragma once
#include "..\GLTFLoader\GLTFLoader.h"

namespace NCL::Rendering {
	class OGLRenderer;
}

namespace Project {
	class Scene;
	class SceneNode;

	using namespace NCL::Rendering;
	using namespace NCL::Maths;
	typedef std::shared_ptr<Scene> SharedScene;
	typedef std::shared_ptr<SceneNode> SharedNode;

	class SceneNode : protected GLTFNode {
	public:
		SceneNode(std::string _name);
		SceneNode(const GLTFNode& base);
		~SceneNode() {};

		void Update(float frameTime);
		void Draw(const OGLRenderer& renderer);

		static std::shared_ptr<SceneNode> FromGLTFScene(GLTFScene& _object, SharedScene parentScene, SharedNode setParent = nullptr);

		void AddChild(std::shared_ptr<SceneNode> child, SharedScene inScene);

		void FindParent(SharedScene inScene);
		void FindChildren(SharedScene inScene);

		bool HasMesh() { return meshID != -1; }
		bool isAnimated() { return animation_id != -1; }

		// TODO: IS TRANSPARENT?? WE CAN DEAL WITH IT WHEN WE HAVE TRANSPARENT MESHES

		std::string GetName() { return name; }
		uint32_t GetID() { return nodeID; }
		SharedMesh GetMesh(SharedScene inScene);
		SharedMeshAnim GetAnimation(SharedScene inScene);
		Matrix4 GetLocalMatrix() { return localMatrix; }
		Matrix4 GetWorldMatrix() { return worldMatrix; }

		// Model Matrix: Transformation from local to world space.
		void SetLocalMatrix(Matrix4 m) { localMatrix = m; }
		void SetAnimation(int32_t anim_id) { animation_id = anim_id; }
	protected:
		//named to not shadow the vars parent and children in the base class
		SharedNode parent_p = nullptr;
		std::vector<SharedNode> children_v;
		int32_t animation_id = -1;
	};
}