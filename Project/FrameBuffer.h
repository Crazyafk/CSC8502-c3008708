#pragma once
#include <memory>
#include "glad\gl.h"

namespace Project {
	using UniqueFrameBuffer = std::unique_ptr<class FrameBuffer>;
	using SharedFrameBuffer = std::shared_ptr<class FrameBuffer>;

	class FrameBuffer {
	public:
		FrameBuffer();
		~FrameBuffer();

		GLuint GetObjectID() const {
			return m_fboID;
		}

		void Bind() const;
		static void Unbind();
	protected:
		GLuint m_fboID;
	};
}