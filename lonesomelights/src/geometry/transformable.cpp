#include "geometry/transformable.h"

Transformable::Transformable(const glm::mat4& local_transformation, const std::shared_ptr<Transformable>& parent_transformable)
	: m_parent_transformable(parent_transformable),
	m_local_transformation(local_transformation) {
}
Transformable::Transformable(const glm::mat4& local_transformation)
	: Transformable(local_transformation, std::shared_ptr<Transformable>()) {
}

const glm::mat4& Transformable::get_local_transformation() const {
	return m_local_transformation;
}
void Transformable::set_local_transformation(const glm::mat4& local_transformation) {
	m_local_transformation = local_transformation;
}

const glm::mat4& Transformable::get_global_transformation() const {
	// TODO: Don't always recalculate here - only recalculate if parent changed.
	// Currently we use this dirty hack until the mentioned thing has been implemented.
	const_cast<Transformable*>(this)->recalculate_global_transformation();
	return m_global_transformation;
}

void Transformable::recalculate_global_transformation() {
	if (!!m_parent_transformable) {
		m_global_transformation = m_parent_transformable->m_local_transformation * m_local_transformation;
	} else {
		m_global_transformation = m_local_transformation;
	}
}