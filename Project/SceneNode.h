#pragma once
#include "..\GLTFLoader\GLTFLoader.h"

namespace Project {
	class Scene;

	using namespace NCL::Rendering;

	class SceneNode : protected GLTFNode {
	public:
		SceneNode(std::string _name, std::shared_ptr<Scene> _scene);
		SceneNode(const GLTFNode& base, std::shared_ptr<Scene> _scene);
		~SceneNode() = default;

		static std::shared_ptr<SceneNode> FromGLTFScene(GLTFScene& _object, std::shared_ptr<Scene> parentScene);

		void AddChild(std::shared_ptr<SceneNode> child);

		void FindParent();
		void FindChildren();

		std::string GetName() { return name; }
		Matrix4 GetLocalMatrix() { return localMatrix; }
		Matrix4 GetWorldMatrix() { return worldMatrix; }
	protected:
		//named to not shadow the vars parent and children in the base class
		std::shared_ptr<SceneNode> parent_p = nullptr;
		std::vector<std::shared_ptr<SceneNode>> children_v;

		std::shared_ptr<Scene> scene = nullptr;
	};
}