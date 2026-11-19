#pragma once
#include "Light.h"
#include "../../NCLCoreClasses/Vector.h"

using namespace NCL::Maths;

namespace Project {
	class PointLight : public Light {
	public:
		PointLight() { type = LightType::Point; }
		PointLight(const Vector3& _position, const Vector4& _colour, float _radius) {
			type = LightType::Point;
			position = _position;
			colour = _colour;
			radius = _radius;
		}
		~PointLight() = default;

		Vector3 GetPosition() const { return position; }
		void SetPosition(const Vector3& _position) { position = _position; }

		float GetRadius() const { return radius; }
		void SetRadius(float _radius) { radius = _radius; }
	protected:
		Vector3 position;
		float radius;
	};
}