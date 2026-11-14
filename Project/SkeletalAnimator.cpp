#include "SkeletalAnimator.h"
#include "../NCLCoreClasses/Mesh.h"
#include "../NCLCoreClasses/MeshAnimation.h"

namespace Project {
	using namespace NCL::Rendering;
	using namespace NCL::Maths;

	SkeletalAnimator::SkeletalAnimator(SharedMesh _mesh, SharedMeshAnim _anim) {
		mesh = _mesh;
		anim = _anim;
		inverseBindPose = mesh->GetInverseBindPose();
		skeleton.resize(anim->GetJointCount());
	}

	void SkeletalAnimator::Update(float frameTime) {
		animTime -= frameTime;

		if (animTime <= 0) {
			animTime += 1.0f / anim->GetFrameRate();
			currentFrame++;
			currentFrame = currentFrame % anim->GetFrameCount();

			std::span<const Matrix4> joints = anim->GetWorldMatrices(currentFrame);
			for (int i = 0; i < skeleton.size(); ++i) {
				skeleton[i] = joints[i] * inverseBindPose[i];
			}
		}
	}
}