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
		void BehaviourUpdate() override;
	};
}