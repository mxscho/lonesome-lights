#include "rendering/opengl/texture.h"

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <utility>
#include <glm/glm.hpp>
#include <png++/png.hpp>

void Texture::unbind_any_from(GLenum texture, GLenum target) {
	glActiveTexture(texture);
	glBindTexture(target, 0);
}
void Texture::unbind_any(GLenum texture) {
	Texture::unbind_any_from(texture, GL_TEXTURE_2D);
}

Texture::Texture(GLenum target)
	: m_id(),
	m_is_generated(false),
	m_target(target) {
}
Texture::Texture(Texture&& texture)
	: m_id(std::move(texture.m_id)),
	m_is_generated(std::move(texture.m_is_generated)),
	m_target(std::move(texture.m_target)) {
	texture.m_is_generated = false;
}
Texture& Texture::operator=(Texture&& texture) {
	std::swap(m_id, texture.m_id);
	std::swap(m_is_generated, texture.m_is_generated);
	std::swap(m_target, texture.m_target);
	return *this;
}
Texture::~Texture() {
	destroy();
}

GLuint Texture::get_id() const {
	assert(m_is_generated);
	return m_id;
}

bool Texture::load_from_file(const std::string& file_path, GLenum texture) {
	destroy();
	
	png::image<png::rgba_pixel> image(file_path.c_str());
	m_width = image.get_width(),
	m_height = image.get_height();
	
	std::vector<glm::vec4> pixels;
	for (unsigned int y = 0; y < m_height; ++y) {
		for (unsigned int x = 0; x < m_width; ++x) {
			png::rgba_pixel rgba_pixel = image.get_pixel(x, m_height - 1 - y);
			pixels.push_back(glm::vec4(rgba_pixel.red, rgba_pixel.green, rgba_pixel.blue, rgba_pixel.alpha));
			// TODO: Check format.
		}
	}
	
	glGenTextures(1, &m_id);
	bind(texture);
	glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(m_target, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_FLOAT, pixels.data());
	Texture::unbind_any_from(texture, m_target);
	
	m_is_generated = true;
	return true;
}

void Texture::bind(GLenum texture) const {
	assert(m_is_generated);
	glActiveTexture(texture);
	glBindTexture(m_target, m_id);
}
void Texture::destroy() {
	if (m_is_generated) {
		glDeleteTextures(1, &m_id);
		m_is_generated = false;
	}
}