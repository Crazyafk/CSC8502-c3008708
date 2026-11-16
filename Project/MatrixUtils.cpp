#include "MatrixUtils.h"

namespace Project {
	Vector3 GetMatrixPosition(Matrix4 mat) {
		return Vector3(
			mat.array[3][0],
			mat.array[3][1],
			mat.array[3][2]
		);
	}

	Matrix4 NoTranslation(const Matrix4& mat) {
		Matrix4 result = mat;
		result.array[3][0] = 0.0f;
		result.array[3][1] = 0.0f;
		result.array[3][2] = 0.0f;
		return result;
	}
}