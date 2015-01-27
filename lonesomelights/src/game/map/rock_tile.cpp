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
				points.push_back(glm::vec3(i_x * quad_size + displacements[i_x].x, i_y * quad_size, displacements[i_x].z));
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
	
	std::vector<RockTile::Data> floor_vertices;
	std::vector<RockTile::Data> side_pos_x; // side 1
	std::vector<RockTile::Data> side_neg_y; // side 2
	std::vector<RockTile::Data> side_neg_x; // side 3
	std::vector<RockTile::Data> side_pos_y; //side 4

	
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
				
				if (i_y == quad_count - 1) {
					if (static_cast<RockTile::CliffType>(i_cliff_type) == RockTile::CliffType::PositiveX) {
						side_pos_x.push_back(RockTile::Data(position_d, glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.0F, 1.0F)));
						side_pos_x.push_back(RockTile::Data(position_c, glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.0F, 1.0F)));
						/*if (first) {
							first_point = RockTile::Data(position_c, glm::vec3(0.0F, 1.0F, 0.0F), texel_c);
							first = false;
						}*/
					} else if (static_cast<RockTile::CliffType>(i_cliff_type) == RockTile::CliffType::NegativeX) {
						side_neg_x.push_back(RockTile::Data(position_a, glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.0F, 1.0F)));
						side_neg_x.push_back(RockTile::Data(position_b, glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.0F, 1.0F)));
						/*if (first) {
							first_point = RockTile::Data(position_d, glm::vec3(0.0F, 1.0F, 0.0F), texel_c);
							first = false;
						}*/
					} else if (static_cast<RockTile::CliffType>(i_cliff_type) == RockTile::CliffType::PositiveY) {
						side_pos_y.push_back(RockTile::Data(position_a, glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.0F, 1.0F)));
						side_pos_y.push_back(RockTile::Data(position_b, glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.0F, 1.0F)));
						/*if (first) {
							first_point = RockTile::Data(position_d, glm::vec3(0.0F, 1.0F, 0.0F), texel_c);
							first = false;
						}*/
						/*if (first) {
							first_point = RockTile::Data(position_d, glm::vec3(0.0F, 1.0F, 0.0F), texel_c);
							first = false;
						}*/
					} else if (static_cast<RockTile::CliffType>(i_cliff_type) == RockTile::CliffType::NegativeY) {
						side_neg_y.push_back(RockTile::Data(position_d, glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.0F, 1.0F)));
						side_neg_y.push_back(RockTile::Data(position_c, glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.0F, 1.0F)));
						/*if (first) {
							first_point = RockTile::Data(position_c, glm::vec3(0.0F, 1.0F, 0.0F), texel_c);
							first = false;
						}*/
					}
				}
			}
		}
	}
	std::vector<unsigned int> top_element_count;
	unsigned int counter = 0;
	top_element_count.push_back(0);

	// None
	floor_vertices.push_back(RockTile::Data(glm::vec3(0.5F, 1.0F, 0.5F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.5F, 0.5F)));
	floor_vertices.push_back(RockTile::Data(glm::vec3(0.0F, 1.0F, 0.0F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.0F, 1.0F)));
	floor_vertices.push_back(RockTile::Data(glm::vec3(0.0F, 1.0F, 1.0F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.0F, 1.0F)));
	floor_vertices.push_back(RockTile::Data(glm::vec3(1.0F, 1.0F, 1.0F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.0F, 1.0F)));
	floor_vertices.push_back(RockTile::Data(glm::vec3(1.0F, 1.0F, 0.0F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.0F, 1.0F)));
	floor_vertices.push_back(RockTile::Data(glm::vec3(0.0F, 1.0F, 0.0F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.0F, 1.0F)));
	counter += 6;
	top_element_count.push_back(counter);
	
	// PositiveX
	floor_vertices.push_back(RockTile::Data(glm::vec3(0.5F, 1.0F, 0.5F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.5F, 0.5F)));
	floor_vertices.push_back(RockTile::Data(glm::vec3(0.0F, 1.0F, 0.0F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.0F, 1.0F)));
	floor_vertices.push_back(RockTile::Data(glm::vec3(0.0F, 1.0F, 1.0F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.0F, 1.0F)));
	for (int i = side_pos_x.size() - 1; i >= 0; --i) {
		floor_vertices.push_back(side_pos_x[i]);
	}
	floor_vertices.push_back(RockTile::Data(glm::vec3(0.0F, 1.0F, 0.0F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.0F, 1.0F)));
	counter += 4 + side_pos_x.size();
	top_element_count.push_back(counter);
	
	// NegativeX
	floor_vertices.push_back(RockTile::Data(glm::vec3(0.5F, 1.0F, 0.5F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.5F, 0.5F)));
	for (unsigned int i = 0; i < side_neg_x.size(); ++i) {
		floor_vertices.push_back(side_neg_x[i]);
	}
	floor_vertices.push_back(RockTile::Data(glm::vec3(1.0F, 1.0F, 1.0F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.0F, 1.0F)));
	floor_vertices.push_back(RockTile::Data(glm::vec3(1.0F, 1.0F, 0.0F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.0F, 1.0F)));
	floor_vertices.push_back(RockTile::Data(glm::vec3(0.0F, 1.0F, 0.0F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.0F, 1.0F)));
	counter += 4 + side_neg_x.size();
	top_element_count.push_back(counter);

	// PositiveY
	floor_vertices.push_back(RockTile::Data(glm::vec3(0.5F, 1.0F, 0.5F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.5F, 0.5F)));
	floor_vertices.push_back(RockTile::Data(glm::vec3(0.0F, 1.0F, 0.0F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.0F, 1.0F)));
	for (unsigned int i = 0; i < side_pos_y.size(); ++i) {
		floor_vertices.push_back(side_pos_y[i]);
	}
	floor_vertices.push_back(RockTile::Data(glm::vec3(1.0F, 1.0F, 0.0F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.0F, 1.0F)));
	floor_vertices.push_back(RockTile::Data(glm::vec3(0.0F, 1.0F, 0.0F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.0F, 1.0F)));
	counter += 4 + side_pos_y.size();
	top_element_count.push_back(counter);
	
	// NegativeY
	floor_vertices.push_back(RockTile::Data(glm::vec3(0.5F, 1.0F, 0.5F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.5F, 0.5F)));
	floor_vertices.push_back(RockTile::Data(glm::vec3(0.0F, 1.0F, 0.0F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.0F, 1.0F)));
	floor_vertices.push_back(RockTile::Data(glm::vec3(0.0F, 1.0F, 1.0F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.0F, 1.0F)));
	floor_vertices.push_back(RockTile::Data(glm::vec3(1.0F, 1.0F, 1.0F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.0F, 1.0F)));
	for (int i = side_neg_y.size() - 1; i >= 0; --i) {
		floor_vertices.push_back(side_neg_y[i]);
	}
	counter += 4 + side_neg_y.size();
	top_element_count.push_back(counter);
	
	// PositiveXNegativeX
	floor_vertices.push_back(RockTile::Data(glm::vec3(0.5F, 1.0F, 0.5F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.5F, 0.5F)));
	for (unsigned int i = 0; i < side_neg_x.size(); ++i) {
		floor_vertices.push_back(side_neg_x[i]);
	}
	for (int i = side_pos_x.size() - 1; i >= 0; --i) {
		floor_vertices.push_back(side_pos_x[i]);
	}
	floor_vertices.push_back(RockTile::Data(glm::vec3(1.0F, 1.0F, 0.0F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.0F, 1.0F)));
	floor_vertices.push_back(RockTile::Data(glm::vec3(0.0F, 1.0F, 0.0F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.0F, 1.0F)));
	counter += 3 + side_neg_x.size() + side_pos_x.size();
	top_element_count.push_back(counter);
	
	// PositiveXPositiveY
	floor_vertices.push_back(RockTile::Data(glm::vec3(0.5F, 1.0F, 0.5F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.5F, 0.5F)));
	floor_vertices.push_back(RockTile::Data(glm::vec3(0.0F, 1.0F, 0.0F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.0F, 1.0F)));
	for (unsigned int i = 0; i < side_pos_y.size(); ++i) {
		floor_vertices.push_back(side_pos_y[i]);
	}
	for (int i = side_pos_x.size() - 1; i >= 0; --i) {
		floor_vertices.push_back(side_pos_x[i]);
	}
	floor_vertices.push_back(RockTile::Data(glm::vec3(0.0F, 1.0F, 0.0F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.0F, 1.0F)));
	counter += 3 + side_pos_y.size() + side_pos_x.size();
	top_element_count.push_back(counter);
	
	// PositiveXNegativeY
	floor_vertices.push_back(RockTile::Data(glm::vec3(0.5F, 1.0F, 0.5F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.5F, 0.5F)));
	floor_vertices.push_back(RockTile::Data(glm::vec3(0.0F, 1.0F, 0.0F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.0F, 1.0F)));
	floor_vertices.push_back(RockTile::Data(glm::vec3(0.0F, 1.0F, 1.0F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.0F, 1.0F)));
	for (int i = side_pos_x.size() - 1; i >= 0; --i) {
		floor_vertices.push_back(side_pos_x[i]);
	}
	for (int i = side_neg_y.size() - 1; i >= 0; --i) {
		floor_vertices.push_back(side_neg_y[i]);
	}
	counter += 3 + side_neg_y.size() + side_pos_x.size();
	top_element_count.push_back(counter);
	
	// NegativeXPositiveY
	floor_vertices.push_back(RockTile::Data(glm::vec3(0.5F, 1.0F, 0.5F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.5F, 0.5F)));
	for (unsigned int i = 0; i < side_neg_x.size(); ++i) {
		floor_vertices.push_back(side_neg_x[i]);
	}
	for (unsigned int i = 0; i < side_pos_y.size(); ++i) {
		floor_vertices.push_back(side_pos_y[i]);
	}
	floor_vertices.push_back(RockTile::Data(glm::vec3(1.0F, 1.0F, 0.0F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.0F, 1.0F)));
	floor_vertices.push_back(RockTile::Data(glm::vec3(0.0F, 1.0F, 0.0F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.0F, 1.0F)));
	counter += 3 + side_pos_y.size() + side_neg_x.size();
	top_element_count.push_back(counter);
	
	// NegativeXNegativeY
	floor_vertices.push_back(RockTile::Data(glm::vec3(0.5F, 1.0F, 0.5F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.5F, 0.5F)));
	for (unsigned int i = 0; i < side_neg_x.size(); ++i) {
		floor_vertices.push_back(side_neg_x[i]);
	}
	floor_vertices.push_back(RockTile::Data(glm::vec3(1.0F, 1.0F, 1.0F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.0F, 1.0F)));
	for (int i = side_neg_y.size() - 1; i >= 0; --i) {
		floor_vertices.push_back(side_neg_y[i]);
	}
	counter += 2 + side_neg_y.size() + side_neg_x.size();
	top_element_count.push_back(counter);
	
	// PositiveYNegativeY
	floor_vertices.push_back(RockTile::Data(glm::vec3(0.5F, 1.0F, 0.5F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.5F, 0.5F)));
	floor_vertices.push_back(RockTile::Data(glm::vec3(0.0F, 1.0F, 0.0F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.0F, 1.0F)));
	for (unsigned int i = 0; i < side_pos_y.size(); ++i) {
		floor_vertices.push_back(side_pos_y[i]);
	}
	for (int i = side_neg_y.size() - 1; i >= 0; --i) {
		floor_vertices.push_back(side_neg_y[i]);
	}
	counter += 2 + side_pos_y.size() + side_neg_y.size();
	top_element_count.push_back(counter);
	
	// PositiveXNegativeXPositiveY
	floor_vertices.push_back(RockTile::Data(glm::vec3(0.5F, 1.0F, 0.5F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.5F, 0.5F)));
	for (unsigned int i = 0; i < side_neg_x.size(); ++i) {
		floor_vertices.push_back(side_neg_x[i]);
	}
	for (unsigned int i = 0; i < side_pos_y.size(); ++i) {
		floor_vertices.push_back(side_pos_y[i]);
	}
	for (int i = side_pos_x.size() - 1; i >= 0; --i) {
		floor_vertices.push_back(side_pos_x[i]);
	}
	floor_vertices.push_back(RockTile::Data(glm::vec3(0.0F, 1.0F, 0.0F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.0F, 1.0F)));
	counter += 2 + side_neg_x.size() + side_pos_x.size() + side_pos_y.size();
	top_element_count.push_back(counter);
	
	// PositiveXNegativeXNegativeY
	floor_vertices.push_back(RockTile::Data(glm::vec3(0.5F, 1.0F, 0.5F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.5F, 0.5F)));
	for (unsigned int i = 0; i < side_neg_x.size(); ++i) {
		floor_vertices.push_back(side_neg_x[i]);
	}
	for (int i = side_pos_x.size() - 1; i >= 0; --i) {
		floor_vertices.push_back(side_pos_x[i]);
	}
	for (int i = side_neg_y.size() - 1; i >= 0; --i) {
		floor_vertices.push_back(side_neg_y[i]);
	}
	counter += 1 + side_neg_y.size() + side_neg_x.size() + side_pos_x.size();
	top_element_count.push_back(counter);
	
	// PositiveXPositiveYNegativeY
	floor_vertices.push_back(RockTile::Data(glm::vec3(0.5F, 1.0F, 0.5F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.5F, 0.5F)));
	floor_vertices.push_back(RockTile::Data(glm::vec3(0.0F, 1.0F, 0.0F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.0F, 1.0F)));
	for (unsigned int i = 0; i < side_pos_y.size(); ++i) {
		floor_vertices.push_back(side_pos_y[i]);
	}
	for (int i = side_pos_x.size() - 1; i >= 0; --i) {
		floor_vertices.push_back(side_pos_x[i]);
	}
	for (int i = side_neg_y.size() - 1; i >= 0; --i) {
		floor_vertices.push_back(side_neg_y[i]);
	}
	counter += 2 + side_neg_y.size() + side_pos_y.size() + side_pos_x.size();
	top_element_count.push_back(counter);
	
	// NegativeXPositiveYNegativeY
	floor_vertices.push_back(RockTile::Data(glm::vec3(0.5F, 1.0F, 0.5F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.5F, 0.5F)));
	for (unsigned int i = 0; i < side_neg_x.size(); ++i) {
		floor_vertices.push_back(side_neg_x[i]);
	}
	for (unsigned int i = 0; i < side_pos_y.size(); ++i) {
		floor_vertices.push_back(side_pos_y[i]);
	}
	for (int i = side_neg_y.size() - 1; i >= 0; --i) {
		floor_vertices.push_back(side_neg_y[i]);
	}
	counter += 1 + side_neg_y.size() + side_neg_x.size() + side_pos_y.size();
	top_element_count.push_back(counter);
	

	// PositiveXNegativeXPositiveYNegativeY
	floor_vertices.push_back(RockTile::Data(glm::vec3(0.5F, 1.0F, 0.5F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec2(0.5F, 0.5F)));
	for (unsigned int i = 0; i < side_neg_x.size(); ++i) {
		floor_vertices.push_back(side_neg_x[i]);
	}
	for (unsigned int i = 0; i < side_pos_y.size(); ++i) {
		floor_vertices.push_back(side_pos_y[i]);
	}
	for (int i = side_pos_x.size() - 1; i >= 0; --i) {
		floor_vertices.push_back(side_pos_x[i]);
	}
	for (int i = side_neg_y.size() - 1; i >= 0; --i) {
		floor_vertices.push_back(side_neg_y[i]);
	}
	counter += 1 + side_neg_y.size() + side_neg_x.size() + side_pos_x.size() + side_pos_y.size();
	top_element_count.push_back(counter);
	
	return RockTile(map, x, y, cliff_type, cliff_vertices, floor_vertices, top_element_count);
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
	if (m_cliff_type == RockTile::CliffType::None) {
		glDrawArrays(GL_TRIANGLE_FAN, m_top_element_count[0], m_top_element_count[1] - m_top_element_count[0]);
	} else if (m_cliff_type == RockTile::CliffType::PositiveX) {
		glDrawArrays(GL_TRIANGLE_FAN, m_top_element_count[1], m_top_element_count[2] - m_top_element_count[1]);
	} else if (m_cliff_type == RockTile::CliffType::NegativeX) {
		glDrawArrays(GL_TRIANGLE_FAN, m_top_element_count[2], m_top_element_count[3] - m_top_element_count[2]);
	} else if (m_cliff_type == RockTile::CliffType::PositiveY) {
		glDrawArrays(GL_TRIANGLE_FAN, m_top_element_count[3], m_top_element_count[4] - m_top_element_count[3]);
	} else if (m_cliff_type == RockTile::CliffType::NegativeY) {
		glDrawArrays(GL_TRIANGLE_FAN, m_top_element_count[4], m_top_element_count[5] - m_top_element_count[4]);
	} else if (m_cliff_type == RockTile::CliffType::PositiveXNegativeX) {
		glDrawArrays(GL_TRIANGLE_FAN, m_top_element_count[5], m_top_element_count[6] - m_top_element_count[5]);
	} else if (m_cliff_type == RockTile::CliffType::PositiveXPositiveY) {
		glDrawArrays(GL_TRIANGLE_FAN, m_top_element_count[6], m_top_element_count[7] - m_top_element_count[6]);
	} else if (m_cliff_type == RockTile::CliffType::PositiveXNegativeY) {
		glDrawArrays(GL_TRIANGLE_FAN, m_top_element_count[7], m_top_element_count[8] - m_top_element_count[7]);
	} else if (m_cliff_type == RockTile::CliffType::NegativeXPositiveY) {
		glDrawArrays(GL_TRIANGLE_FAN, m_top_element_count[8], m_top_element_count[9] - m_top_element_count[8]);
	} else if (m_cliff_type == RockTile::CliffType::NegativeXNegativeY) {
		glDrawArrays(GL_TRIANGLE_FAN, m_top_element_count[9], m_top_element_count[10] - m_top_element_count[9]);
	} else if (m_cliff_type == RockTile::CliffType::PositiveYNegativeY) {
		glDrawArrays(GL_TRIANGLE_FAN, m_top_element_count[10], m_top_element_count[11] - m_top_element_count[10]);
	} else if (m_cliff_type == RockTile::CliffType::PositiveXNegativeXPositiveY) {
		glDrawArrays(GL_TRIANGLE_FAN, m_top_element_count[11], m_top_element_count[12] - m_top_element_count[11]);
	} else if (m_cliff_type == RockTile::CliffType::PositiveXNegativeXNegativeY) {
		glDrawArrays(GL_TRIANGLE_FAN, m_top_element_count[12], m_top_element_count[13] - m_top_element_count[12]);
	} else if (m_cliff_type == RockTile::CliffType::PositiveXPositiveYNegativeY) {
		glDrawArrays(GL_TRIANGLE_FAN, m_top_element_count[13], m_top_element_count[14] - m_top_element_count[13]);
	} else if (m_cliff_type == RockTile::CliffType::NegativeXPositiveYNegativeY) {
		glDrawArrays(GL_TRIANGLE_FAN, m_top_element_count[14], m_top_element_count[15] - m_top_element_count[14]);
	} else if (m_cliff_type == RockTile::CliffType::PositiveXNegativeXPositiveYNegativeY) {
		glDrawArrays(GL_TRIANGLE_FAN, m_top_element_count[15], m_top_element_count[16] - m_top_element_count[15]);
	}
	
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

RockTile::RockTile(const Map& map, unsigned int x, unsigned int y, const CliffType& cliff_type, const std::vector<RockTile::Data>& cliff_vertices, const std::vector<RockTile::Data>& floor_vertices, const std::vector<unsigned int>& top_element_count)
	: Tile(map, x, y, RenderPrograms::get_render_program("rock_tile")),
	m_cliff_type(cliff_type),
	m_cliff_vertices_vbo(cliff_vertices, GL_ARRAY_BUFFER),
	m_cliff_vao(),
	m_floor_vertices_vbo(floor_vertices, GL_ARRAY_BUFFER),
	m_floor_vao(),
	m_floor_tile(FloorTile(map, x, y)),
	m_color(glm::vec4(0.0, 0.0f, 0.0f, 0.0f)),
	m_top_element_count(top_element_count) {
	
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
