#include "game/units/worker_unit.h"

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

std::unique_ptr<WorkerUnit> WorkerUnit::create(const glm::vec2& position, const Map& map, const Player& player, unsigned int id) {
	std::vector<GLfloat> vine_positions = ObjLoader::get_obj_positions("unit_0", 0);
	std::vector<GLfloat> vine_normals = ObjLoader::get_obj_normals("unit_0", 0);
	std::vector<GLfloat> ball_positions = ObjLoader::get_obj_positions("unit_0", 1);
	std::vector<GLfloat> ball_normals = ObjLoader::get_obj_normals("unit_0", 1);
	
	std::vector<WorkerUnit::Data> vertices;
	std::vector<unsigned int> vertex_counts;
	for (unsigned int i = 0; i < vine_positions.size(); i += 3) {
		vertices.push_back(WorkerUnit::Data(glm::vec3(vine_positions[i + 0], vine_positions[i + 1], vine_positions[i + 2]), glm::vec3(vine_normals[i + 0], vine_normals[i + 1], vine_normals[i + 2])));
	}
	vertex_counts.push_back(vine_positions.size() / 3);
	for (unsigned int i = 0; i < ball_positions.size(); i += 3) {
		vertices.push_back(WorkerUnit::Data(glm::vec3(ball_positions[i + 0], ball_positions[i + 1], ball_positions[i + 2]), glm::vec3(ball_normals[i + 0], ball_normals[i + 1], ball_normals[i + 2])));
	}
	vertex_counts.push_back(ball_positions.size() / 3);
	
	return std::unique_ptr<WorkerUnit>(new WorkerUnit(position, map, player, vertices, vertex_counts, id));
}

float WorkerUnit::get_attack_range() const {
	return 1.4F;
}
float WorkerUnit::get_attack_dps() const {
	return 5.0F;
}

Tile* WorkerUnit::get_exploited() const {
	return m_exploited;
}
void WorkerUnit::start_exploiting(CrystalTile* exploited) {
	m_exploited = exploited;
	m_laser.set_enabled(true);
}
void WorkerUnit::start_exploiting(DestructibleRockTile* exploited) {
	m_exploited = exploited;
	m_laser.set_enabled(true);
}
void WorkerUnit::stop_exploiting() {
	m_exploited = nullptr;
	m_laser.set_enabled(false);
}

float WorkerUnit::get_crystal_count() const {
	return m_crystal_count;
}

void WorkerUnit::set_crystal_count(float new_crystal_count) {
		m_crystal_count = std::max(0.0f, new_crystal_count);
}

void WorkerUnit::change_crystal_count(float delta) {
	m_crystal_count = std::max(0.0f, m_crystal_count + delta);
}

void WorkerUnit::draw(const Camera& camera) const {
	Drawable::draw(camera);
	
	Unit::draw(camera);

	Drawable::m_render_program.set_uniform("u_model_transformation", Transformable::get_global_transformation());
	Drawable::m_render_program.set_uniforms("u_view_transformation", "u_projection_transformation", "u_camera_eye_position", "u_camera_up_direction", camera);
	Drawable::m_render_program.set_uniform("u_color", Unit::m_player.get_color());

	Drawable::m_render_program.bind();
	
	Drawable::m_render_program.set_uniform("u_model_transformation", Transformable::get_global_transformation() * m_vine_transformation);
	m_vine_vao.bind();
	glDrawElements(GL_TRIANGLES, m_vine_elements_vbo.get_size(), GL_UNSIGNED_INT, nullptr);
	
	Drawable::m_render_program.set_uniform("u_color", glm::vec3(0.32f, 0.9f, 0.4f));
	
	Drawable::m_render_program.set_uniform("u_model_transformation", Transformable::get_global_transformation() * m_ball_transformation);
	m_ball_vao.bind();
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glDrawElementsBaseVertex(GL_TRIANGLES, m_ball_elements_vbo.get_size(), GL_UNSIGNED_INT, nullptr, m_vertex_counts[0]);
	glDisable(GL_BLEND);
	
	VertexArrayObject::unbind_any();
	RenderProgram::unbind_any();
	
	if (m_crystal_count > 0.0f) {
		m_crystal.draw(camera);
	}
}
void WorkerUnit::draw_laser(const Camera& camera) const {
	m_flash.draw(camera);
	m_laser.draw(camera);
}
void WorkerUnit::draw_deferred(const Camera& camera, const Texture& color_texture, const Texture& position_texture, const Texture& normal_texture, const Texture& depth_texture) const {
	m_flash.draw_deferred(camera, color_texture, position_texture, normal_texture, depth_texture);
	m_laser.draw_deferred(camera, color_texture, position_texture, normal_texture, depth_texture);
}

void WorkerUnit::update(const Timer& timer) {
	Unit::update(timer);
	
	float health_percent = Attackable::get_current_health() / Attackable::get_max_health();
	if (health_percent < 1.0F) {
		m_flash.set_start_color(glm::vec3(0.16f, 0.45f, 0.2f) * health_percent);
		m_flash.set_end_color(glm::vec3(0.3F, 0.3F, 0.3F) *  health_percent);
	}
	m_flash.update(timer);
	
	m_laser.Transformable::set_position(Transformable::get_position());
	if (m_exploited) {
		m_laser.set_target(glm::vec3(m_exploited->get_x() + 0.5F, 0.5F, m_exploited->get_y() + 0.5F));
	}
	m_laser.update(timer);

	m_crystal.update(timer);
	glm::vec3 unit_position = InertialMovable::get_position();
	m_crystal.set_position(glm::vec3(unit_position.x, 0.6F, unit_position.z));
	
	float scale_factor = 0.6F + (sin(timer.get_current_time_seconds() * 2.0F) + 1.0F) / 2.0F * 0.4F;
	m_ball_transformation = glm::scale(glm::vec3(scale_factor, scale_factor, scale_factor));
	float rotation_angle = timer.get_current_time_seconds();
	m_vine_transformation = glm::rotate(rotation_angle, glm::vec3(0.0F, 1.0F, 0.0F));

}

WorkerUnit::Data::Data(const glm::vec3& position, const glm::vec3& normal)
	: position(position),
	normal(normal) {
}

WorkerUnit::WorkerUnit(const glm::vec2& position, const Map& map, const Player& player, const std::vector<WorkerUnit::Data>& vertices, const std::vector<unsigned int>& vertex_counts, unsigned int id)
	: Drawable(RenderPrograms::get_render_program("unit")),
	Unit(glm::translate(glm::vec3(0.0F, 0.3F, 0.0F)) * glm::scale(glm::vec3(0.0065F, 0.0065F, 0.0065F)), position, map, player,
	1.2F, // Maximum velocity
	0.9F, 0.9F, // Acceleration/Decceleration
	100.0F, // Maximum health
	id),
	m_vertices_vbo(vertices, GL_ARRAY_BUFFER),
	m_vertex_counts(vertex_counts),
	m_vine_elements_vbo(ObjLoader::get_obj_elements("unit_0", 0), GL_ELEMENT_ARRAY_BUFFER),
	m_ball_elements_vbo(ObjLoader::get_obj_elements("unit_0", 1), GL_ELEMENT_ARRAY_BUFFER),
	m_vine_vao(),
	m_ball_vao(),
	m_vine_transformation(),
	m_ball_transformation(),

	m_laser(glm::translate(Transformable::get_position()), map, glm::vec3(0.2F, 0.6F, 0.2F)),
	m_exploited(nullptr),
	m_crystal(Crystal::create(glm::translate(glm::vec3(position.x, 0.6F, position.y)) * glm::scale(glm::vec3(0.07f, 0.07f, 0.07f)), map)),
	m_crystal_count(0.0f),
	m_flash(
		glm::mat4(), // Transformation
		*this, // Parent transformable
		glm::vec2(0.75 * 1.0F, 0.75F * 1.0F), // Billboard size
		Textures::get_texture("particles/flash"), // Texture
		glm::vec3(0.16f, 0.45f, 0.2f), // Start color
		glm::vec3(0.3F, 0.3F, 0.3F), // End color
		1.35F, // Radius
		0.75F * 0.05F, // Particle start velocity
		0.0F, // Gravity
		0.15F, // Minimum particle lifetime (seconds)
		0.65F, // Maximum particle lifetime (seconds)
		0.75 * 10.0F  // Frequency
	) {
	
	m_vine_vao.bind();
	m_vertices_vbo.bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(WorkerUnit::Data), (void*) ((0 * 3) * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(WorkerUnit::Data), (void*) ((1 * 3) * sizeof(GL_FLOAT)));
	m_vine_elements_vbo.bind();
	
	m_ball_vao.bind();
	m_vertices_vbo.bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(WorkerUnit::Data), (void*) ((0 * 3) * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(WorkerUnit::Data), (void*) ((1 * 3) * sizeof(GL_FLOAT)));
	m_ball_elements_vbo.bind();
	
	VertexArrayObject::unbind_any();
	VertexBufferObjects::unbind_any();
}
