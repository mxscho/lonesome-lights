#include "geometry/transformable.h"

Transformable::Transformable(const glm::mat4& local_transformation, const Transformable& parent_transformable)
	: m_parent_transformable(&parent_transformable),
	m_local_transformation(local_transformation),
	m_parent_transformation(),
	m_global_transformation() {
}
Transformable::Transformable(const glm::mat4& local_transformation)
	: m_parent_transformable(nullptr),
	m_local_transformation(local_transformation),
	m_parent_transformation(),
	m_global_transformation() {
}

const glm::mat4& Transformable::get_local_transformation() const {
	return m_local_transformation;
}
void Transformable::set_local_transformation(const glm::mat4& local_transformation) {
	m_local_transformation = local_transformation;
}
glm::vec3 Transformable::get_position() const {
	glm::vec3 position;
	position.x = m_local_transformation[3][0];
	position.y = m_local_transformation[3][1];
	position.z = m_local_transformation[3][2];
	return position;
}
void Transformable::set_position(const glm::vec3& position) {
	m_local_transformation[3][0] = position.x;
	m_local_transformation[3][1] = position.y;
	m_local_transformation[3][2] = position.z;
}

const glm::mat4& Transformable::get_global_transformation() const {
	// TODO: Don't always recalculate here - only recalculate if parent changed.
	// Currently we use this dirty hack until the mentioned thing has been implemented.
	const_cast<Transformable*>(this)->recalculate_global_transformation();
	return m_global_transformation;
}

glm::mat4 Transformable::get_global_transformation(const glm::mat4& additional_local_transformation) const {
	// TODO: Don't always recalculate here - only recalculate if parent changed.
	// Currently we use this dirty hack until the mentioned thing has been implemented.
	const_cast<Transformable*>(this)->recalculate_global_transformation();
	return m_parent_transformation * additional_local_transformation * m_local_transformation;
}

void Transformable::recalculate_global_transformation() {
	if (!!m_parent_transformable) {
		// TODO: Don't always recalculate parent here.
		// Currently we use this dirty hack.
		const_cast<Transformable*>(m_parent_transformable)->recalculate_global_transformation();
		m_parent_transformation = m_parent_transformable->m_global_transformation;
	} else {
		m_parent_transformation = glm::mat4();
	}
	m_global_transformation = m_parent_transformation * m_local_transformation;
}