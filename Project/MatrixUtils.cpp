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
		Matrix4 result;
		result.array[0][0] = mat.array[0][0];
		result.array[0][1] = mat.array[0][1];
		result.array[0][2] = mat.array[0][2];
		result.array[1][0] = mat.array[1][0];
		result.array[1][1] = mat.array[1][1];
		result.array[1][2] = mat.array[1][2];
		result.array[2][0] = mat.array[2][0];
		result.array[2][1] = mat.array[2][1];
		result.array[2][2] = mat.array[2][2];
		return result;
	}
}