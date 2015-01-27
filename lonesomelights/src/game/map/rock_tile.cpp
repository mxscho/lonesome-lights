#include "game/map/rock_tile.h"

#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>

#include "game/map/map.h"
#include "rendering/opengl/render_program.h"
#include "rendering/opengl/render_programs.h"
#include "rendering/opengl/textures.h"


//test
#include <iostream>

RockTile RockTile::create(const Map& map, unsigned int x, unsigned int y, const CliffType& cliff_type) {
	float tile_size = map.get_tile_size();
	
	unsigned int quad_count = 6;
	float quad_size = tile_size / quad_count;
		
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> displacements(quad_count + 1, glm::vec3());
	std::vector<int> signs(quad_count + 1);
	for (unsigned int i_x = 0; i_x < quad_count + 1; ++i_x) {
		signs[i_x] = rand() % 2 == 1 ? 1 : -1;
	}
	for (unsigned int i_y = 0; i_y < quad_count + 1; ++i_y) {
		for (unsigned int i_x = 0; i_x < quad_count + 1; ++i_x) {
			if (i_y != 0 && i_x != 0 && i_x != (quad_count + 1) - 1) {
				displacements[i_x] += glm::vec3(
					((float) rand() / RAND_MAX) * 0.02F - 0.01F,
					((float) rand() / RAND_MAX) * 0.02F - 0.01F,
					signs[i_x] * ((float) rand() / RAND_MAX) * 0.025F
				);
			}
			if (i_y < quad_count) {
				points.push_back(glm::vec3(i_x * quad_size + displacements[i_x].x, i_y * quad_size + displacements[i_x].y, displacements[i_x].z));
			} else {
				points.push_back(glm::vec3(i_x * quad_size, i_y * quad_size, 0.0F));
			}
		}
	}

	std::vector<glm::vec3> normals(quad_count);
	for (unsigned int i_x = 0; i_x < quad_count; ++i_x) {
		glm::vec3 normal;
		for (unsigned int i_y = 0; i_y < quad_count; ++i_y) {
			// D - C
			// |   |
			// A - B
			
			glm::vec3 position_a = points[(i_y) * (quad_count + 1) + (i_x)];
			glm::vec3 position_b = points[(i_y) * (quad_count + 1) + (i_x + 1)];
			glm::vec3 position_c = points[(i_y + 1) * (quad_count + 1) + (i_x + 1)];
			glm::vec3 position_d = points[(i_y + 1) * (quad_count + 1) + (i_x)];
			
			normal += glm::normalize(glm::cross(
				position_a - position_b,
				position_c - position_b
			));
			normal += glm::normalize(glm::cross(
				position_c - position_d,
				position_a - position_d
			));
		}
		normal /= quad_count * 2;
		normals[i_x] = normal;
	}
	
	std::vector<RockTile::Data> cliff_vertices;
	for (unsigned int i_cliff_type = 1; i_cliff_type <= 8; i_cliff_type *= 2) {
		for (unsigned int i_y = 0; i_y < quad_count; ++i_y) {
			for (unsigned int i_x = 0; i_x < quad_count; ++i_x) {
			
				// D - C
				// |   |
				// A - B
				
				glm::vec3 position_a = points[(i_y) * (quad_count + 1) + (i_x)];
				glm::vec2 texel_a = glm::vec2(position_a.x, position_a.y);
				glm::vec3 position_b = points[(i_y) * (quad_count + 1) + (i_x + 1)];
				glm::vec2 texel_b = glm::vec2(position_b.x, position_b.y);
				glm::vec3 position_c = points[(i_y + 1) * (quad_count + 1) + (i_x + 1)];
				glm::vec2 texel_c = glm::vec2(position_c.x, position_c.y);
				glm::vec3 position_d = points[(i_y + 1) * (quad_count + 1) + (i_x)];
				glm::vec2 texel_d = glm::vec2(position_d.x, position_d.y);
				
				glm::vec3 normal = normals[i_x];
				
				if (static_cast<RockTile::CliffType>(i_cliff_type) == RockTile::CliffType::PositiveX) {
					position_a = glm::vec3(position_a.z, position_a.y, position_a.x);
					position_b = glm::vec3(position_b.z, position_b.y, position_b.x);
					position_c = glm::vec3(position_c.z, position_c.y, position_c.x);
					position_d = glm::vec3(position_d.z, position_d.y, position_d.x);

					position_a.x += 1.0F;
					position_b.x += 1.0F;
					position_c.x += 1.0F;
					position_d.x += 1.0F;
					
					normal = glm::vec3(-normal.z, normal.y, normal.x);
				} else if (static_cast<RockTile::CliffType>(i_cliff_type) == RockTile::CliffType::NegativeX) {
					position_a = glm::vec3(position_a.z, position_a.y, position_a.x);
					position_b = glm::vec3(position_b.z, position_b.y, position_b.x);
					position_c = glm::vec3(position_c.z, position_c.y, position_c.x);
					position_d = glm::vec3(position_d.z, position_d.y, position_d.x);
					
					glm::vec3 tmp = position_a;
					position_a = position_d;
					position_d = tmp;
					
					tmp = position_b;
					position_b = position_c;
					position_c = tmp;
					normal = glm::vec3(normal.z, normal.y, -normal.x);
				} else if (static_cast<RockTile::CliffType>(i_cliff_type) == RockTile::CliffType::PositiveY) {
					glm::vec3 tmp = position_a;
					position_a = position_d;
					position_d = tmp;
					
					tmp = position_b;
					position_b = position_c;
					position_c = tmp;
					
					position_a.z += 1.0F;
					position_b.z += 1.0F;
					position_c.z += 1.0F;
					position_d.z += 1.0F;
					normal = -normal;
				} else if (static_cast<RockTile::CliffType>(i_cliff_type) == RockTile::CliffType::NegativeY) {
					// Default.
				}

				cliff_vertices.push_back(RockTile::Data(position_a, normal, texel_a));
				cliff_vertices.push_back(RockTile::Data(position_c, normal, texel_c));
				cliff_vertices.push_back(RockTile::Data(position_b, normal, texel_b));

				cliff_vertices.push_back(RockTile::Data(position_c, normal, texel_c));
				cliff_vertices.push_back(RockTile::Data(position_a, normal, texel_a));
				cliff_vertices.push_back(RockTile::Data(position_d, normal, texel_d));
			}
		}
	}
	
	std::vector<RockTile::Data> floor_vertices = {
		RockTile::Data(glm::vec3(0.0F, 1.0F, 0.0F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.0F, 0.0F)),
		RockTile::Data(glm::vec3(0.0F, 1.0F, 1.0F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.0F, 1.0F)),
		RockTile::Data(glm::vec3(1.0F, 1.0F, 1.0F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(1.0F, 1.0F)),
		RockTile::Data(glm::vec3(1.0F, 1.0F, 0.0F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(1.0F, 0.0F))
	};

	return RockTile(map, x, y, cliff_type, cliff_vertices, floor_vertices);
}

void RockTile::draw(const Camera& camera) const {
	m_floor_tile.draw(camera);

	Drawable::draw(camera);

	Drawable::m_render_program.set_uniform("u_model_transformation", Transformable::get_global_transformation());
	Drawable::m_render_program.set_uniforms("u_view_transformation", "u_projection_transformation", "u_camera_eye_position", "u_camera_up_direction", camera);

	Drawable::m_render_program.bind();
	
	const Texture& texture = Textures::get_texture("rock_tile");
	Drawable::m_render_program.set_uniform("u_color", m_color);
	Drawable::m_render_program.set_uniform("u_texture", 0);
	
	texture.bind(GL_TEXTURE0);

	m_cliff_vao.bind();
	unsigned int cliff_vertex_count = m_cliff_vertices_vbo.get_size() / 4;
	if ((static_cast<unsigned int>(m_cliff_type) & static_cast<unsigned int>(RockTile::CliffType::PositiveX)) != 0) {
		glDrawArrays(GL_TRIANGLES, 0 * cliff_vertex_count, cliff_vertex_count);
	}
	if ((static_cast<unsigned int>(m_cliff_type) & static_cast<unsigned int>(RockTile::CliffType::NegativeX)) != 0) {
		glDrawArrays(GL_TRIANGLES, 1 * cliff_vertex_count, cliff_vertex_count);
	}
	if ((static_cast<unsigned int>(m_cliff_type) & static_cast<unsigned int>(RockTile::CliffType::PositiveY)) != 0) {
		glDrawArrays(GL_TRIANGLES, 2 * cliff_vertex_count, cliff_vertex_count);
	}
	if ((static_cast<unsigned int>(m_cliff_type) & static_cast<unsigned int>(RockTile::CliffType::NegativeY)) != 0) {
		glDrawArrays(GL_TRIANGLES, 3 * cliff_vertex_count, cliff_vertex_count);
	}
	m_floor_vao.bind();
	unsigned int floor_vertex_count = m_floor_vertices_vbo.get_size() / 4;
	glDrawArrays(GL_QUADS, 0, 4 * floor_vertex_count);
	
	Texture::unbind_any(GL_TEXTURE0);
	VertexArrayObject::unbind_any();
	RenderProgram::unbind_any();
}

void RockTile::set_color(glm::vec3 new_color) {
	m_color = glm::vec4(new_color.x, new_color.y, new_color.z, 1.0f);
}

void RockTile::set_cliff_type(unsigned short cliff_type) {
	m_cliff_type = static_cast<RockTile::CliffType>(cliff_type);
}

RockTile::CliffType RockTile::get_cliff_type() const {
	return m_cliff_type;
}

bool RockTile::is_rock() const {
	return true;
}

RockTile::Data::Data(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texel)
	: position(position),
	normal(normal),
	texel(texel) {
}

RockTile::RockTile(const Map& map, unsigned int x, unsigned int y, const CliffType& cliff_type, const std::vector<RockTile::Data>& cliff_vertices, const std::vector<RockTile::Data>& floor_vertices)
	: Tile(map, x, y, RenderPrograms::get_render_program("rock_tile")),
	m_cliff_type(cliff_type),
	m_cliff_vertices_vbo(cliff_vertices, GL_ARRAY_BUFFER),
	m_cliff_vao(),
	m_floor_vertices_vbo(floor_vertices, GL_ARRAY_BUFFER),
	m_floor_vao(),
	m_floor_tile(FloorTile(map, x, y)),
	m_color(glm::vec4(0.0, 0.0f, 0.0f, 0.0f)) {
	
	Tile::set_is_walkable(false);
	
	m_cliff_vao.bind();
	m_cliff_vertices_vbo.bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(RockTile::Data), (void*) ((0 * 3 + 0 * 2) * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(RockTile::Data), (void*) ((1 * 3 + 0 * 2) * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(RockTile::Data), (void*) ((2 * 3 + 0 * 2) * sizeof(GL_FLOAT)));
	
	m_floor_vao.bind();
	m_floor_vertices_vbo.bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(RockTile::Data), (void*) ((0 * 3 + 0 * 2) * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(RockTile::Data), (void*) ((1 * 3 + 0 * 2) * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(RockTile::Data), (void*) ((2 * 3 + 0 * 2) * sizeof(GL_FLOAT)));
	VertexArrayObject::unbind_any();
	VertexBufferObjects::unbind_any();
}
