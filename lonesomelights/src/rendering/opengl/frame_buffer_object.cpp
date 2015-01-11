#include "rendering/opengl/frame_buffer_object.h"

#include <cassert>
#include <utility>

void FrameBufferObject::unbind_any() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBufferObject::FrameBufferObject(unsigned int width, unsigned int height)
	: m_width(width),
	m_height(height),
	m_is_generated(true),
	m_id(),
	m_number_of_colorbuffers(0),
	m_drawBuffers(),
	m_drawbuffers_set(true) {
	
	glGenFramebuffers(1, &m_id);
}
FrameBufferObject::FrameBufferObject(FrameBufferObject&& frame_buffer_object)
	: m_is_generated(std::move(frame_buffer_object.m_is_generated)),
	m_id(std::move(frame_buffer_object.m_id)) {
	frame_buffer_object.m_is_generated = false;
}
FrameBufferObject& FrameBufferObject::operator=(FrameBufferObject&& frame_buffer_object) {
	std::swap(m_is_generated, frame_buffer_object.m_is_generated);
	std::swap(m_id, frame_buffer_object.m_id);
	return *this;
}
FrameBufferObject::~FrameBufferObject() {
	if (m_is_generated) {
		glDeleteFramebuffers(1, &m_id);
		m_is_generated = false;
	}
}

GLuint FrameBufferObject::get_id() const {
	assert(m_is_generated);
	return m_id;
}

bool FrameBufferObject::check_status() const {
	assert(m_is_generated);
	return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

void FrameBufferObject::attach_depthbuffer_from_texture(GLuint texture) {
	assert(m_is_generated);
	// TODO: bind texture
	attach_texture(GL_DEPTH_ATTACHMENT, texture);
}

void FrameBufferObject::attach_colorbuffer_from_texture(GLuint texture) {
	assert(m_is_generated);
	// TODO: bind texture
	m_drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + m_number_of_colorbuffers);
	attach_texture(m_drawBuffers[m_number_of_colorbuffers], texture);
	m_number_of_colorbuffers++;
	m_drawbuffers_set = false;
}

void FrameBufferObject::setDrawbuffers() {
	if (m_number_of_colorbuffers > 0) {
		glDrawBuffers(m_number_of_colorbuffers, m_drawBuffers.data());
	} else {
		glDrawBuffer(GL_NONE);
	}
	m_drawbuffers_set = true;
}

void FrameBufferObject::bind() {
	assert(m_is_generated);
	glBindFramebuffer(GL_FRAMEBUFFER, m_id);
	// render in the whole framebuffer from bottom left to top right
	glViewport(0, 0, m_width, m_height);
	if (!m_drawbuffers_set) {
		setDrawbuffers();
	}
}

void FrameBufferObject::attach_texture(GLenum attachment, GLuint texture) {
	assert(m_is_generated);
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture, 0);

}