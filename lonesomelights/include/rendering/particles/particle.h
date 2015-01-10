#ifndef __RENDERING__PARTICLES__PARTICLE_H__
#define __RENDERING__PARTICLES__PARTICLE_H__

#include <glm/glm.hpp>

#include "rendering/opengl/vertex_buffer_object.h"

class Particle {
public:
	struct Data {
		Data(const glm::vec3& start_position, const glm::vec3& start_velocity, const glm::vec3& acceleration, float start_time_seconds, float lifetime_seconds);
		Data();
	
		glm::vec3 start_position;
		glm::vec3 start_velocity;
		glm::vec3 acceleration;
		float start_time_seconds;
		float lifetime_seconds;
	};
	
	Particle(VertexBufferObject<Data>::Bucket&& vertex_buffer_object_bucket, const glm::vec3& start_position, const glm::vec3& start_velocity, const glm::vec3& acceleration, float start_time_seconds, float lifetime_seconds);
	
	bool is_alive(float current_time) const;
private:
	VertexBufferObject<Data>::Bucket m_vertex_buffer_object_bucket;
};

#endif