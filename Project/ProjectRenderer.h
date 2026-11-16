#pragma once
#include "SceneManagement/SceneRenderer.h"

namespace Project {
	using namespace NCL::Rendering;
	class ProjectRenderer : public SceneRenderer {
	public:
		ProjectRenderer();
		~ProjectRenderer() = default;
	protected:
		std::vector<std::string> GetSkyboxTexturePaths() override;
		void PopulateScene() override;
		void BeforeFirstFrame() override;
		void BehaviourUpdate() override;

		float cameraSpeed = 30.0f;
		Vector3 cameraMoveVector = { 0, 0, -cameraSpeed }; //don't ask why -z, yaw-relative, is forwards. just is.
		float waypointChangeDist = 0.5f;
		int nextWaypoint = 0;

		void UpdateCameraPath();
		bool isCameraAtWaypoint();
		void OnWaypointArrival();

		std::vector<Vector3> cameraPath = {
			{ 12.8988, -6.59071, -198.503 },
			{ -9.95711, -6.59071, 147.466 },
			{ -29.6057, -4.34487, 246.853 },
			{ -51.0739, -2.09138, 319.129 },
			{ -97.3196, 1.66133, 424.395 },
			{ -211.652, 5.41516, 506.792 },
			{ -268.779, 8.91403, 534.77 },
			{ -267.456, 13.246, 590.155 },
			{ -276.139, 14.9149, 614.779 },
			{ -405.162, 14.5021, 693.44 },
			{ -544.152, 14.5021, 781.048 },
			{ -653.769, 34.3359, 793.902 }
		};
	};
}