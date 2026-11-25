#include "FrameBuffer.h"

namespace Project {
	FrameBuffer::FrameBuffer() {
		glGenFramebuffers(1, &m_fboID);
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