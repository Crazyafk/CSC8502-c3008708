#include "FrameBuffer.h"

namespace Project {
	FrameBuffer::FrameBuffer(SharedOGLTexture _colour, SharedOGLTexture _depth, SharedOGLTexture _stencil) {
		glGenFramebuffers(1, &m_fboID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);

		if (_colour) {
			colour = _colour;
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_2D, colour->GetObjectID(), 0);
		}
		else {
			glDrawBuffer(GL_NONE);
		}
		if (_depth) {
			depth = _depth;
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
				GL_TEXTURE_2D, depth->GetObjectID(), 0);
		}
		if (_stencil) {
			stencil = _stencil;
			//TODO: LOGIC
		}
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	FrameBuffer::~FrameBuffer() {
		glDeleteFramebuffers(1, &m_fboID);
	}

	void FrameBuffer::Bind() const {
		glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);
	}

	void FrameBuffer::Unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}