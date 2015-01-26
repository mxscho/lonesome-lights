#ifndef __GEOMETRY__CAMERA_H__
#define __GEOMETRY__CAMERA_H__

#include <glm/glm.hpp>
#include <string>

class RenderProgram;

class Camera {
public:
	const glm::vec3& get_eye_position() const;
	const glm::vec3& get_up_direction() const;
	const glm::vec3 get_look_direction() const;
	const glm::mat4& get_view_transformation() const;
	const glm::mat4& get_projection_transformation() const;
protected:
	static Camera create_from_view_direction(const glm::vec3& eye_position, const glm::vec3& view_direction, const glm::vec3& up_direction, const glm::mat4& projection_transformation);

	Camera(const glm::vec3& eye_position, const glm::vec3& center_position, const glm::vec3& up_direction, const glm::mat4& projection_transformation);
	
	void change_position(const glm::vec3& offset);
	
	glm::vec3 m_eye_position;
	glm::vec3 m_center_position;
	glm::vec3 m_up_direction;
private:
	glm::mat4 m_view_transformation;
	glm::mat4 m_projection_transformation;
};

#endif
