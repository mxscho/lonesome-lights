#include "rendering/particles/particle_emitter.h"

#include "rendering/opengl/render_program.h"
#include "rendering/opengl/render_programs.h"
#include "rendering/opengl/texture.h"
#include "timer.h"

#include <cassert>

ParticleEmitter::ParticleEmitter(const glm::mat4& transformation, const Transformable& parent_transformable, const std::string& fragment_shader_name, const glm::vec2& billboard_size, bool orientate_towards_velocity, unsigned int max_particle_count)
	: DeferredDrawable(RenderPrograms::get_render_program("particle_emitter", fragment_shader_name), RenderPrograms::get_render_program("deferred_particle_emitter")),
	Updatable(),
	Transformable(transformation, parent_transformable),
	m_particle_offset(),
	m_particle_start_velocity(),
	m_particle_acceleration(),
	m_particle_lifetime_seconds(),
	m_frequency(),
	m_is_emitting(true),
	m_orientate_towards_velocity(orientate_towards_velocity),
	m_current_time_seconds(),
	m_max_particle_count(max_particle_count),
	m_base_vertex_buffer_object({ glm::vec2(-billboard_size.x / 2.0F, -billboard_size.y / 2.0F), glm::vec2(billboard_size.x / 2.0F, -billboard_size.y / 2.0F), glm::vec2(billboard_size.x / 2.0F, billboard_size.y / 2.0F), glm::vec2(-billboard_size.x / 2.0F, billboard_size.y / 2.0F) }, GL_ARRAY_BUFFER),
	m_instances_vertex_buffer_object(m_max_particle_count, GL_ARRAY_BUFFER),
	m_particles(),
	m_next_emission_time_seconds(-1.0F),
	m_vertex_array_object() {
	m_vertex_array_object.bind();
	m_base_vertex_buffer_object.bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	m_instances_vertex_buffer_object.bind();
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle::Data), (void*) ((0 * 3 + 0 * 1) * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Particle::Data), (void*) ((1 * 3 + 0 * 1) * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Particle::Data), (void*) ((2 * 3 + 0 * 1) * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Particle::Data), (void*) ((3 * 3 + 0 * 1) * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(Particle::Data), (void*) ((3 * 3 + 1 * 1) * sizeof(GL_FLOAT)));
	VertexArrayObject::unbind_any();
	VertexBufferObjects::unbind_any();
}

void ParticleEmitter::set_emitting(bool is_emitting) {
	m_is_emitting = is_emitting;
}

void ParticleEmitter::draw(const Camera& camera) const {
	Drawable::draw(camera);

	Drawable::m_render_program.set_uniform("u_model_transformation", Transformable::get_global_transformation());
	Drawable::m_render_program.set_uniforms("u_view_transformation", "u_projection_transformation", "u_camera_eye_position", "u_camera_up_direction", camera);
	Drawable::m_render_program.set_uniform("u_orientate_towards_velocity", m_orientate_towards_velocity);
	Drawable::m_render_program.set_uniform("u_current_time_seconds", m_current_time_seconds);
	
	Drawable::m_render_program.bind();
	m_vertex_array_object.bind();

	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glDrawArraysInstanced(GL_QUADS, 0, 4, m_max_particle_count);
	
	VertexArrayObject::unbind_any();
	RenderProgram::unbind_any();
}
void ParticleEmitter::draw_deferred(const Camera& camera, const Texture& color_texture, const Texture& position_texture, const Texture& normal_texture, const Texture& depth_texture) const {
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	
	DeferredDrawable::draw_deferred(camera, color_texture, position_texture, normal_texture, depth_texture);
	
	DeferredDrawable::m_deferred_render_program.set_uniform("u_color_texture", 0);
	DeferredDrawable::m_deferred_render_program.set_uniform("u_position_texture", 1);
	DeferredDrawable::m_deferred_render_program.set_uniform("u_normal_texture", 2);
	DeferredDrawable::m_deferred_render_program.set_uniform("u_depth_texture", 3);
	
	DeferredDrawable::m_deferred_render_program.set_uniform("u_model_transformation", Transformable::get_global_transformation());
	DeferredDrawable::m_deferred_render_program.set_uniforms("u_view_transformation", "u_projection_transformation", "u_camera_eye_position", "u_camera_up_direction", camera);
	DeferredDrawable::m_deferred_render_program.set_uniform("u_orientate_towards_velocity", false);
	DeferredDrawable::m_deferred_render_program.set_uniform("u_current_time_seconds", m_current_time_seconds);
	DeferredDrawable::m_deferred_render_program.set_uniform("u_lighting_intensity", 0.05F);
	
	DeferredDrawable::m_deferred_render_program.bind();
	m_vertex_array_object.bind();
	
	color_texture.bind(GL_TEXTURE0);
	position_texture.bind(GL_TEXTURE1);
	normal_texture.bind(GL_TEXTURE2);
	depth_texture.bind(GL_TEXTURE3);
	
	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glDrawArraysInstanced(GL_QUADS, 0, 4, m_max_particle_count);
	
	Texture::unbind_any(GL_TEXTURE0);
	Texture::unbind_any(GL_TEXTURE1);
	Texture::unbind_any(GL_TEXTURE2);
	Texture::unbind_any(GL_TEXTURE3);
	
	VertexArrayObject::unbind_any();
	RenderProgram::unbind_any();
	
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
}

void ParticleEmitter::update(const Timer& timer) {
	Updatable::update(timer);
	
	m_current_time_seconds = timer.get_current_time_seconds();
	if (m_next_emission_time_seconds == -1.0F) {
		m_next_emission_time_seconds = m_current_time_seconds;
	}
		
	for (auto i_particle = m_particles.begin(); i_particle != m_particles.end(); ) {
		if (i_particle->is_alive(m_current_time_seconds)) {
			++i_particle;
		} else {
			i_particle = m_particles.erase(i_particle);
		}
	}
	while (m_next_emission_time_seconds <= m_current_time_seconds) {
		assert(m_particles.size() < m_max_particle_count);
		recalculate_properties();
		if (m_is_emitting) {
			m_particles.push_back(Particle(m_instances_vertex_buffer_object.claim_bucket(), m_particle_offset, m_particle_start_velocity, m_particle_acceleration, m_next_emission_time_seconds, m_particle_lifetime_seconds));
		}
		m_next_emission_time_seconds += 1.0F / m_frequency;
	}
	
	m_instances_vertex_buffer_object.update();
}