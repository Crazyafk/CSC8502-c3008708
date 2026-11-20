#pragma once
#include "Light.h"
#include "../../NCLCoreClasses/Vector.h"

using namespace NCL::Maths;

namespace Project {
	class DirectionLight : public Light {
	public:
		DirectionLight() { type = LightType::Direction; }
		DirectionLight(Vector3 _direction, Vector4 _colour) {
			type = LightType::Direction;
			colour = _colour;
			direction = Vector::Normalise(_direction);
		}
		~DirectionLight() = default;

		void SetDirection(Vector3 _direction) { direction = Vector::Normalise(_direction); }
		Vector3 GetDirection() const { return direction; }

		void AddShadows(Vector3 _mapOrigin) { shadowMapOrigin = _mapOrigin; hasShadows = true; }
		bool HasShadows() const { return hasShadows; }
		Vector3 GetShadowMapOrigin() const { return shadowMapOrigin; }
	protected:
		Vector3 direction; //defined as the incident vector
		bool hasShadows;
		Vector3 shadowMapOrigin; //the position from which the shadow map is drawn if set
	};
}