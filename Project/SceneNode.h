#pragma once
#include "..\GLTFLoader\GLTFLoader.h"

namespace Project {
	class Scene;
	class SceneNode;

	using namespace NCL::Rendering;
	typedef std::shared_ptr<Scene> SharedScene;
	typedef std::shared_ptr<SceneNode> SharedNode;

	class SceneNode : protected GLTFNode {
	public:
		SceneNode(std::string _name, std::shared_ptr<Scene> _scene);
		SceneNode(const GLTFNode& base, std::shared_ptr<Scene> _scene);
		~SceneNode() {};

		static std::shared_ptr<SceneNode> FromGLTFScene(GLTFScene& _object, SharedScene parentScene, SharedNode setParent = nullptr);

		void AddChild(std::shared_ptr<SceneNode> child);

		void FindParent();
		void FindChildren();

		//bool HasMesh() { return meshID == -1; }

		std::string GetName() { return name; }
		uint32_t GetID() { return nodeID; }
		//SharedMesh GetMesh() { return HasMesh() ? nullptr : scene->GetMesh(meshID); }
		Matrix4 GetLocalMatrix() { return localMatrix; }
		Matrix4 GetWorldMatrix() { return worldMatrix; }
	protected:
		//named to not shadow the vars parent and children in the base class
		std::shared_ptr<SceneNode> parent_p = nullptr;
		std::vector<std::shared_ptr<SceneNode>> children_v;

		std::shared_ptr<Scene> scene = nullptr;
	};
}