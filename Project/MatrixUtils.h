#pragma once
#include "..\NCLCoreClasses\Matrix.h"

namespace Project {
	using namespace NCL::Maths;
	extern Vector3 GetMatrixPosition(Matrix4 mat);
	extern Matrix4 NoTranslation(const Matrix4& mat);
}