#include "rendering/opengl/vertex_array_object.h"

#include <cassert>
#include <utility>

void VertexArrayObject::unbind_any() {
	glBindVertexArray(0);
}

VertexArrayObject::VertexArrayObject()
	: m_is_generated(true),
	m_id() {
	glGenVertexArrays(1, &m_id);
}
VertexArrayObject::VertexArrayObject(VertexArrayObject&& vertex_array_object)
	: m_is_generated(std::move(vertex_array_object.m_is_generated)),
	m_id(std::move(vertex_array_object.m_id)) {
	vertex_array_object.m_is_generated = false;
}
VertexArrayObject& VertexArrayObject::operator=(VertexArrayObject&& vertex_array_object) {
	std::swap(m_is_generated, vertex_array_object.m_is_generated);
	std::swap(m_id, vertex_array_object.m_id);
	return *this;
}
VertexArrayObject::~VertexArrayObject() {
	if (m_is_generated) {
		glDeleteVertexArrays(1, &m_id);
		m_is_generated = false;
	}
}

GLuint VertexArrayObject::get_id() const {
	assert(m_is_generated);
	return m_id;
}

void VertexArrayObject::bind() const {
	assert(m_is_generated);
	glBindVertexArray(m_id);
}