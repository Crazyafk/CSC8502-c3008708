#pragma once
#include "../NCLCoreClasses/Matrix.h"
#include "../NCLCoreClasses/Mesh.h"
#include "../NCLCoreClasses/MeshAnimation.h"

namespace Project {
	using namespace NCL::Rendering;
	using namespace NCL::Maths;

	struct SkeletalAnimator {
		SkeletalAnimator(SharedMesh _mesh, SharedMeshAnim _anim);
		~SkeletalAnimator() = default;

		void Update(float frameTime);

		SharedMesh mesh;
		SharedMeshAnim anim;
		std::vector<Matrix4> inverseBindPose;

		std::vector<Matrix4> skeleton;
		int currentFrame = 0;
		float animTime = 0.0f;
	};

	typedef std::shared_ptr<SkeletalAnimator> SharedSkeletalAnimator;
}