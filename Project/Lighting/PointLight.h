#pragma once

#include "../../NCLCoreClasses/Vector.h"

using namespace NCL::Maths;

namespace Project {
	class PointLight {
	public:
		PointLight() {}
		PointLight(const Vector3& _position, const Vector4& _colour, float _radius) {
			position = _position;
			colour = _colour;
			radius = _radius;
		}
		~PointLight() = default;

		Vector3 GetPosition() const { return position; }
		void SetPosition(const Vector3& _position) { position = _position; }

		float GetRadius() const { return radius; }
		void SetRadius(float _radius) { radius = _radius; }

		Vector4 GetColour() const { return colour; }
		void SetColour(const Vector4& _colour) { colour = _colour; }
	protected:
		Vector3 position;
		float radius;
		Vector4 colour;
	};
}