#pragma once
#include <memory>
#include "glad\gl.h"
#include "../OpenGLRendering/OGLTexture.h"

using namespace NCL::Rendering;

namespace Project {
	using UniqueFrameBuffer = std::unique_ptr<class FrameBuffer>;
	using SharedFrameBuffer = std::shared_ptr<class FrameBuffer>;

	class FrameBuffer {
	public:
		FrameBuffer(SharedOGLTexture _colour = nullptr, SharedOGLTexture _depth = nullptr, SharedOGLTexture _stencil = nullptr);
		~FrameBuffer();

		GLuint GetObjectID() const {
			return m_fboID;
		}
		SharedOGLTexture GetColour() const { return colour; }
		SharedOGLTexture GetDepth() const { return depth; }
		SharedOGLTexture GetStencil() const { return stencil; }

		void Bind() const;
		static void Unbind();
	protected:
		GLuint m_fboID;
		SharedOGLTexture colour;
		SharedOGLTexture depth;
		SharedOGLTexture stencil;
	};
}