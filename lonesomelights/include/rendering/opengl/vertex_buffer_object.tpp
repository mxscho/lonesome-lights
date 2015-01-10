#include <cassert>
#include <utility>

#include "rendering/opengl/vertex_buffer_objects.h"

template<typename T>
VertexBufferObject<T>::Bucket::Bucket(VertexBufferObject<T>& vertex_buffer_object, unsigned int index)
	: m_vertex_buffer_object(vertex_buffer_object),
	m_index(index),
	m_in_use(true) {
}
template<typename T>
VertexBufferObject<T>::Bucket::Bucket(VertexBufferObject<T>::Bucket&& bucket)
	: m_vertex_buffer_object(bucket.m_vertex_buffer_object),
	m_index(std::move(bucket.m_index)),
	m_in_use(std::move(bucket.m_in_use)) {
	bucket.m_in_use = false;
}
template<typename T>
typename VertexBufferObject<T>::Bucket& VertexBufferObject<T>::Bucket::operator=(VertexBufferObject<T>::Bucket&& bucket) {
	std::swap(m_vertex_buffer_object, bucket.m_vertex_buffer_object);
	std::swap(m_index, bucket.m_index);
	std::swap(m_in_use, bucket.m_in_use);
	return *this;
}
template<typename T>
VertexBufferObject<T>::Bucket::~Bucket() {
	if (m_in_use) {
		m_vertex_buffer_object.m_unused_indices.push_back(m_index);
		m_in_use = false;
	}
}

template<typename T>
const T& VertexBufferObject<T>::Bucket::get() const {
	return m_vertex_buffer_object.m_data[m_index];
}
template<typename T>
void VertexBufferObject<T>::Bucket::set(const T& value) {
	m_vertex_buffer_object.m_data[m_index] = value;
	
	if (m_vertex_buffer_object.m_changed_begin_index > m_index) {
		m_vertex_buffer_object.m_changed_begin_index = m_index;
	}
	if (m_vertex_buffer_object.m_changed_end_index < m_index + 1) {
		m_vertex_buffer_object.m_changed_end_index = m_index + 1;
	}
}

template<typename T>
VertexBufferObject<T>::VertexBufferObject(const std::vector<T>& data, GLenum target)
	: VertexBufferObject(data.size(), target, GL_STATIC_DRAW, data) {
}
template<typename T>
VertexBufferObject<T>::VertexBufferObject(unsigned int size, GLenum target)
	: VertexBufferObject(size, target, GL_DYNAMIC_DRAW, std::vector<T>(size)) {
}
template<typename T>
VertexBufferObject<T>::VertexBufferObject(VertexBufferObject<T>&& vertex_buffer_object)
	: m_id(std::move(vertex_buffer_object.m_id)),
	m_is_generated(std::move(vertex_buffer_object.m_is_generated)),
	m_target(std::move(vertex_buffer_object.m_target)),
	m_usage(std::move(vertex_buffer_object.m_usage)),
	m_size(std::move(vertex_buffer_object.m_size)),
	m_data(std::move(vertex_buffer_object.m_data)),
	m_changed_begin_index(std::move(vertex_buffer_object.m_changed_begin_index)),
	m_changed_end_index(std::move(vertex_buffer_object.m_changed_end_index)),
	m_unused_indices(std::move(vertex_buffer_object.m_unused_indices)) {
	vertex_buffer_object.m_is_generated = false;
}
template<typename T>
VertexBufferObject<T>& VertexBufferObject<T>::operator=(VertexBufferObject<T>&& vertex_buffer_object) {
	std::swap(m_id, vertex_buffer_object.m_id);
	std::swap(m_is_generated, vertex_buffer_object.m_is_generated);
	std::swap(m_target, vertex_buffer_object.m_target);
	std::swap(m_usage, vertex_buffer_object.m_usage);
	std::swap(m_size, vertex_buffer_object.m_size);
	std::swap(m_data, vertex_buffer_object.m_data);
	std::swap(m_changed_begin_index, vertex_buffer_object.m_changed_begin_index);
	std::swap(m_changed_end_index, vertex_buffer_object.m_changed_end_index);
	std::swap(m_unused_indices, vertex_buffer_object.m_unused_indices);
}
template<typename T>
VertexBufferObject<T>::~VertexBufferObject() {
	destroy();
}

template<typename T>
typename VertexBufferObject<T>::Bucket VertexBufferObject<T>::claim_bucket() {
	unsigned int unused_index = m_unused_indices.front();
	m_unused_indices.pop_front();
	return VertexBufferObject<T>::Bucket(*this, unused_index);
}

template<typename T>
void VertexBufferObject<T>::bind() const {
	assert(m_is_generated);
	glBindBuffer(m_target, m_id);
}
template<typename T>
void VertexBufferObject<T>::update() {
	assert(m_is_generated);
	if (m_changed_begin_index >= m_changed_end_index) {
		return;
	}
	
	bind();
	unsigned int changed_size = m_changed_end_index - m_changed_begin_index;
	if (changed_size == m_size) {
		glBufferData(m_target, m_size * sizeof(T), m_data.data(), m_usage);
	} else {
		glBufferSubData(m_target, m_changed_begin_index * sizeof(T), changed_size * sizeof(T), &m_data[m_changed_begin_index]);
	}
	VertexBufferObjects::unbind_any_from(m_target);
	
	m_changed_begin_index = m_size;
	m_changed_end_index = 0;
}
template<typename T>
void VertexBufferObject<T>::destroy() {
	if (m_is_generated) {
		glDeleteBuffers(1, &m_id);
		m_is_generated = false;
	}
}

template<typename T>
VertexBufferObject<T>::VertexBufferObject(unsigned int size, GLenum target, GLenum usage, const std::vector<T>& data)
	: m_id(),
	m_is_generated(true),
	m_target(target),
	m_usage(usage),
	m_size(size),
	m_data(data),
	m_changed_begin_index(m_size),
	m_changed_end_index(0),
	m_unused_indices() {
	glGenBuffers(1, &m_id);
	bind();
	glBufferData(m_target, m_size * sizeof(T), m_data.data(), m_usage);
	VertexBufferObjects::unbind_any_from(m_target);
	for (unsigned int i = 0; i < m_size; ++i) {
		m_unused_indices.push_back(i);
	}
}