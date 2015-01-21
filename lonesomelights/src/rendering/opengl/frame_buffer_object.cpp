#include "rendering/opengl/frame_buffer_object.h"

#include <cassert>
#include <utility>

#include "rendering/opengl/texture.h"

void FrameBufferObject::unbind_any() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBufferObject::FrameBufferObject(unsigned int width, unsigned int height)
	: m_width(width),
	m_height(height),
	m_is_generated(true),
	m_id(),
	m_colorbuffers_count(0),
	m_drawbuffers() {
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

void FrameBufferObject::attach_depthbuffer_from_texture(const Texture& texture) {
	assert(m_is_generated);
	texture.bind(GL_TEXTURE0);
	attach_texture(GL_DEPTH_ATTACHMENT, texture);
	Texture::unbind_any(GL_TEXTURE0);
}

void FrameBufferObject::attach_colorbuffer_from_texture(const Texture& texture) {
	assert(m_is_generated);
	texture.bind(GL_TEXTURE0);
	m_drawbuffers.push_back(GL_COLOR_ATTACHMENT0 + m_colorbuffers_count);
	attach_texture(m_drawbuffers[m_colorbuffers_count], texture);
	Texture::unbind_any(GL_TEXTURE0);
	++m_colorbuffers_count;
}

void FrameBufferObject::finalize_attaching() {
	glDrawBuffers(m_colorbuffers_count, m_drawbuffers.data());
}

void FrameBufferObject::bind() {
	assert(m_is_generated);
	glBindFramebuffer(GL_FRAMEBUFFER, m_id);
}

void FrameBufferObject::attach_texture(GLenum attachment, const Texture& texture) {
	assert(m_is_generated);
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture.get_id(), 0);
}