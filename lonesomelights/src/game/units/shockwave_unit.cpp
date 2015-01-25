#include "game/units/shockwave_unit.h"

#include "game/map/map.h"
#include "game/player.h"
#include "rendering/obj_loader/obj_loader.h"
#include "rendering/opengl/render_program.h"
#include "rendering/opengl/render_programs.h"
#include "rendering/opengl/textures.h"
#include "timer.h"

#include <cmath>
#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>

std::unique_ptr<ShockwaveUnit> ShockwaveUnit::create(const glm::vec2& position, const Map& map, const Player& player) {
	std::vector<GLfloat> vine_positions = ObjLoader::get_obj_positions("unit_0", 0);
	std::vector<GLfloat> vine_normals = ObjLoader::get_obj_normals("unit_0", 0);
	std::vector<GLfloat> ball_positions = ObjLoader::get_obj_positions("unit_0", 1);
	std::vector<GLfloat> ball_normals = ObjLoader::get_obj_normals("unit_0", 1);
	
	std::vector<ShockwaveUnit::Data> vertices;
	std::vector<unsigned int> vertex_counts;
	for (unsigned int i = 0; i < vine_positions.size(); i += 3) {
		vertices.push_back(ShockwaveUnit::Data(glm::vec3(vine_positions[i + 0], vine_positions[i + 1], vine_positions[i + 2]), glm::vec3(vine_normals[i + 0], vine_normals[i + 1], vine_normals[i + 2])));
	}
	vertex_counts.push_back(vine_positions.size() / 3);
	for (unsigned int i = 0; i < ball_positions.size(); i += 3) {
		vertices.push_back(ShockwaveUnit::Data(glm::vec3(ball_positions[i + 0], ball_positions[i + 1], ball_positions[i + 2]), glm::vec3(ball_normals[i + 0], ball_normals[i + 1], ball_normals[i + 2])));
	}
	vertex_counts.push_back(ball_positions.size() / 3);
	
	return std::unique_ptr<ShockwaveUnit>(new ShockwaveUnit(position, map, player, vertices, vertex_counts));
}

float ShockwaveUnit::get_attack_range() const {
	return 2.0F;
}
float ShockwaveUnit::get_attack_dps() const {
	return 10.0F;
}

const std::set<Attackable*>& ShockwaveUnit::get_attacked() const {
	return m_attacked;
}
void ShockwaveUnit::add_attack(Attackable* attackable) {
	m_attacked.insert(attackable);
}
void ShockwaveUnit::remove_attack(Attackable* attackable) {
	for (auto i_attacked = m_attacked.begin(); i_attacked != m_attacked.end(); ) {
		if (*i_attacked == attackable) {
			i_attacked = m_attacked.erase(i_attacked);
		} else {
			++i_attacked;
		}
	}
}
void ShockwaveUnit::clear_attacks() {
	m_attacked.clear();
}

void ShockwaveUnit::draw(const Camera& camera) const {
	Drawable::draw(camera);
	
	if (m_attacked.size() > 0) {
		m_shockwave.draw(camera);
	}

	Unit::draw(camera);

	Drawable::m_render_program.set_uniform("u_model_transformation", Transformable::get_global_transformation());
	Drawable::m_render_program.set_uniforms("u_view_transformation", "u_projection_transformation", "u_camera_eye_position", "u_camera_up_direction", camera);
	Drawable::m_render_program.set_uniform("u_color", Unit::m_player.get_color());

	Drawable::m_render_program.bind();
	
	Drawable::m_render_program.set_uniform("u_model_transformation", Transformable::get_global_transformation() * m_vine_transformation);
	m_vine_vao.bind();
	glDrawElements(GL_TRIANGLES, m_vine_elements_vbo.get_size(), GL_UNSIGNED_INT, nullptr);
	
	Drawable::m_render_program.set_uniform("u_model_transformation", Transformable::get_global_transformation() * m_ball_transformation);
	m_ball_vao.bind();
	glDrawElementsBaseVertex(GL_TRIANGLES, m_ball_elements_vbo.get_size(), GL_UNSIGNED_INT, nullptr, m_vertex_counts[0]);
	
	VertexArrayObject::unbind_any();
	RenderProgram::unbind_any();
}
void ShockwaveUnit::draw_shockwave(const Camera& camera) const {
}
/*void ShockwaveUnit::draw_deferred(const Camera& camera, const Texture& color_texture, const Texture& position_texture, const Texture& normal_texture, const Texture& depth_texture) const {
	// TODO: draw shockwave deferred
}*/

void ShockwaveUnit::update(const Timer& timer) {
	Unit::update(timer);
	
	
	m_shockwave.Transformable::set_position(glm::vec3(Transformable::get_position().x, 0.001F, Transformable::get_position().z));
	// TODO: Maybe set on/off if attackables available.
	m_shockwave.update(timer);
	
	float scale_factor = 0.6F + (sin(timer.get_current_time_seconds() * 2.0F) + 1.0F) / 2.0F * 0.4F;
	m_ball_transformation = glm::scale(glm::vec3(scale_factor, scale_factor, scale_factor));
	float rotation_angle = timer.get_current_time_seconds();
	m_vine_transformation = glm::rotate(rotation_angle, glm::vec3(0.0F, 1.0F, 0.0F));
	
	// TEST
	// Networkable::on_update();
}

ShockwaveUnit::Data::Data(const glm::vec3& position, const glm::vec3& normal)
	: position(position),
	normal(normal) {
}

ShockwaveUnit::ShockwaveUnit(const glm::vec2& position, const Map& map, const Player& player, const std::vector<ShockwaveUnit::Data>& vertices, const std::vector<unsigned int>& vertex_counts)
	: Drawable(RenderPrograms::get_render_program("unit")),
	Unit(glm::translate(glm::vec3(0.0F, 0.3F, 0.0F)) * glm::scale(glm::vec3(0.0075F, 0.0075F, 0.0075F)), position, map, player,
	0.8F, // Maximum velocity
	0.5F, 0.5F, // Acceleration/Decceleration
	100.0F // Maximum health
	), // TODO: find nice values for shockwave unit
	m_vertices_vbo(vertices, GL_ARRAY_BUFFER),
	m_vertex_counts(vertex_counts),
	m_vine_elements_vbo(ObjLoader::get_obj_elements("unit_0", 0), GL_ELEMENT_ARRAY_BUFFER),
	m_ball_elements_vbo(ObjLoader::get_obj_elements("unit_0", 1), GL_ELEMENT_ARRAY_BUFFER),
	m_vine_vao(),
	m_ball_vao(),
	m_vine_transformation(),
	m_ball_transformation(),

	m_shockwave(glm::translate(glm::vec3(Transformable::get_position().x, 0.001F, Transformable::get_position().z)) * glm::scale(glm::vec3(2.0F * get_attack_range(), 1.0F, 2.0F * get_attack_range())), map, Unit::m_player.get_color()),
	m_attacked() {
	
	m_vine_vao.bind();
	m_vertices_vbo.bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ShockwaveUnit::Data), (void*) ((0 * 3) * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ShockwaveUnit::Data), (void*) ((1 * 3) * sizeof(GL_FLOAT)));
	m_vine_elements_vbo.bind();
	
	m_ball_vao.bind();
	m_vertices_vbo.bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ShockwaveUnit::Data), (void*) ((0 * 3) * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ShockwaveUnit::Data), (void*) ((1 * 3) * sizeof(GL_FLOAT)));
	m_ball_elements_vbo.bind();
	
	VertexArrayObject::unbind_any();
	VertexBufferObjects::unbind_any();
}
