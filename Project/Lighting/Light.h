#pragma once
#include "../../NCLCoreClasses/Vector.h"

using namespace NCL::Maths;

namespace Project {
	enum class LightType: int{
		Invalid = 0,
		Point = 1,
		Direction = 2
	};
	class Light {
	public:
		LightType GetType() const { return type; }

		Vector4 GetColour() const { return colour; }
		void SetColour(const Vector4& _colour) { colour = _colour; }
	protected:
		LightType type;
		Vector4 colour;
	};
	typedef std::shared_ptr<Light> SharedLight;
}