#include "ProjectRenderer.h"
#include "SceneManagement/SceneUtils.h"

namespace Project {
	ProjectRenderer::ProjectRenderer() : SceneRenderer() {}

	std::vector<std::string> ProjectRenderer::GetSkyboxTexturePaths() {
		return {
			NCL::Assets::TEXTUREDIR + "Cubemap/skyrender0004.png",
			NCL::Assets::TEXTUREDIR + "Cubemap/skyrender0001.png",
			NCL::Assets::TEXTUREDIR + "Cubemap/skyrender0003.png",
			NCL::Assets::TEXTUREDIR + "Cubemap/skyrender0006.png",
			NCL::Assets::TEXTUREDIR + "Cubemap/skyrender0002.png",
			NCL::Assets::TEXTUREDIR + "Cubemap/skyrender0005.png"
		};
	}

	void ProjectRenderer::PopulateScene() {
		light = std::make_unique<PointLight>(
			Vector3(-18.4, -4.17863, -48.3475),
			Vector4(1, 1, 1, 1),
			10.0f
		);

		GLTFScene tempSceneA;
		GLTFLoader::Load("../GLTF/CesiumMan/CesiumMan.gltf", tempSceneA);
		SharedNode cesiumMan = SceneNode::FromGLTFScene(tempSceneA, scene);
		cesiumMan->SetLocalMatrix(Matrix::Translation(Vector3{ 0,0,0 })
			* Matrix::Rotation(90.0f, Vector3(0, 1, 0))
			* Matrix::Rotation(270.0f, Vector3(1, 0, 0))
			* Matrix::Scale(Vector3{ 1,1,1 }));
		SharedNode cesiumManMesh;
		cesiumMan->FindMeshNode(cesiumManMesh);
		cesiumManMesh->SetAnimation(0, scene);

		GLTFScene tempSceneB;
		GLTFLoader::Load("../GLTF/DamagedHelmet/DamagedHelmet.gltf", tempSceneB);
		SharedNode helmet = SceneNode::FromGLTFScene(tempSceneB, scene);
		helmet->SetLocalMatrix(Matrix::Translation(Vector3{ -1, 1.5, 0 })
			* Matrix::Rotation(180.0f, Vector3(0, 1, 0))
			* Matrix::Rotation(90.0f, Vector3(1, 0, 0))
			* Matrix::Scale(Vector3{ 0.5f,0.5f,0.5f }));

		GLTFScene tempSceneC;
		GLTFLoader::Load("../GLTF/BugattiTrack/scene.gltf", tempSceneC);
		CentreNodes(tempSceneC);
		SharedNode world = SceneNode::FromGLTFScene(tempSceneC, scene);
		world->SetLocalMatrix(Matrix::Translation(Vector3{ 0, -10, 0 })
			* Matrix::Rotation(180.0f, Vector3(0, 1, 0))
			* Matrix::Rotation(270.0f, Vector3(1, 0, 0))
			* Matrix::Scale(Vector3{ 1,1,1 }));

		scene->UploadMeshes(this);
	}

	void ProjectRenderer::BeforeFirstFrame() {
		SceneRenderer::BeforeFirstFrame();

		defaultCamera->SetPosition(cameraPath[0]);
		OnWaypointArrival();
	}

	void ProjectRenderer::BehaviourUpdate() {
		UpdateCameraPath();
	}

	void ProjectRenderer::UpdateCameraPath() {
		if (!doCamPath) return;
		if (nextWaypoint == -1) return;
		if (isCameraAtWaypoint()) OnWaypointArrival();
		defaultCamera->MoveYawRelative(cameraMoveVector * frameTime, true);
	}

	bool ProjectRenderer::isCameraAtWaypoint() {
		Vector3 diff = defaultCamera->GetPosition() - cameraPath[nextWaypoint];
		return (Maths::Vector::Length(diff) < waypointChangeDist);
	}

	void ProjectRenderer::OnWaypointArrival() {
		nextWaypoint++;
		if (nextWaypoint == cameraPath.size()) { 
			nextWaypoint = -1;
			return;
		}
		defaultCamera->LookTowards(cameraPath[nextWaypoint]);
	}
}