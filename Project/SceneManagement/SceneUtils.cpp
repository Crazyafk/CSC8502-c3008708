#include "SceneUtils.h"

namespace Project {
	void CentreNodes(GLTFScene& _object) {
		std::unordered_map<int, Vector3> meshAverages;
		for (int i = 0; i < _object.meshes.size(); ++i) {
			meshAverages[i] = _object.meshes[i]->Centre();
		}
		for (GLTFNode& node : _object.sceneNodes) {
			if (node.meshID == -1) continue;
			node.localMatrix = node.localMatrix * Matrix::Translation(meshAverages[node.meshID]);
		}
	}
}