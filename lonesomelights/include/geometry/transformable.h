#ifndef __GEOMETRY_TRANSFORMABLE_H__
#define __GEOMETRY_TRANSFORMABLE_H__

#include <memory>
#include <glm/glm.hpp>

class Transformable {
public:
	Transformable(const glm::mat4& local_transformation, const Transformable& parent_transformable);
	explicit Transformable(const glm::mat4& local_transformation);

	const glm::mat4& get_local_transformation() const;
	void set_local_transformation(const glm::mat4& local_transformation);
	glm::vec3 get_position() const;
	void set_position(const glm::vec3& position);
	
	const glm::mat4& get_global_transformation() const;
private:
	void recalculate_global_transformation();

	const Transformable* m_parent_transformable;
	glm::mat4 m_local_transformation;
	glm::mat4 m_global_transformation;
};

#endif