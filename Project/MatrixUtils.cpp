#include "MatrixUtils.h"

namespace Project {
	Vector3 GetMatrixPosition(Matrix4 mat) {
		return Vector3(
			mat.array[3][0],
			mat.array[3][1],
			mat.array[3][2]
		);
	}
}