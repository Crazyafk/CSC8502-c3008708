#pragma once

namespace Project {
	enum LightType : int {
		Point
	};
	class Light {
	public:
		LightType GetType() const { return type; }
	protected:
		LightType type;
	};
}