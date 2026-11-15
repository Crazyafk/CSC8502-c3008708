#pragma once
#include "..\GLTFLoader\GLTFLoader.h"
#include "SkeletalAnimator.h"

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

		static std::shared_ptr<SceneNode> FromGLTFScene(GLTFScene& _object, SharedScene parentScene, SharedNode setParent = nullptr);

		void AddChild(std::shared_ptr<SceneNode> child, SharedScene inScene);

		void FindParent(SharedScene inScene);
		void FindChildren(SharedScene inScene);

		bool HasMesh() { return meshID != -1; }
		bool isAnimated() { return animation_id != -1; }
		bool isTransparent() { return is_transparent; };

		// TODO: IS TRANSPARENT?? WE CAN DEAL WITH IT WHEN WE HAVE TRANSPARENT MESHES

		std::vector<SharedNode> GetChildren() { return children_v; }

		std::string GetName() { return name; }
		uint32_t GetID() { return nodeID; }
		SharedMesh GetMesh(SharedScene inScene);
		GLTFMeshMaterials GetMeshMaterials(SharedScene inScene);
		SharedMeshAnim GetAnimation(SharedScene inScene);
		Matrix4 GetLocalMatrix() { return localMatrix; }
		Matrix4 GetWorldMatrix() { return worldMatrix; }
		SharedSkeletalAnimator GetAnimator() { return animator; }

		// Finds the first node with a mesh from itself or descendants, depth-first
		bool FindMeshNode(SharedNode& out);

		// Model Matrix: Transformation from local to world space.
		void SetLocalMatrix(Matrix4 m) { localMatrix = m; }
		void SetAnimation(int32_t anim_id, SharedScene inScene) { 
			animation_id = anim_id; 
			animator = std::make_shared<SkeletalAnimator>(GetMesh(inScene), GetAnimation(inScene));
		}

		float GetCameraDistance() const { return distanceFromCamera; }
		void SetCameraDistance(float f) { 
			if (distanceFromCameraOverriden) return;
			distanceFromCamera = f; 
		}
		void OverrideCameraDistance(float f) { distanceFromCamera = f; distanceFromCameraOverriden = true; }
		static bool CompareByCameraDistance(SharedNode a, SharedNode b) {
			return(a->distanceFromCamera < b->distanceFromCamera);
		}
	protected:
		//named to not shadow the vars parent and children in the base class
		SharedNode parent_p = nullptr;
		std::vector<SharedNode> children_v;
		int32_t animation_id = -1;
		float distanceFromCamera;
		bool distanceFromCameraOverriden = false;
		SharedSkeletalAnimator animator = nullptr;
		bool is_transparent = false;

		void FindIsTransparent(SharedScene inScene);
	};
}