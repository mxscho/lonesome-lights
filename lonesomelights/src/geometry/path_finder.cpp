#include "geometry/path_finder.h"
#include <iostream>
#include <vector>

PathFinder::PathFinder(Map& map, unsigned int tile_resolution)
 : m_map(map),
	m_tile_resolution(tile_resolution){
	
	if (m_tile_resolution < 1) {
		m_tile_resolution = 1;
	}
	m_tile_size = m_map.get_tile_size();
	m_tile_count_x = m_map.get_tile_count_x();
	m_tile_count_y = m_map.get_tile_count_y();
	
	// initialize graph data
	for (unsigned int i_tiles_x = 0; i_tiles_x < m_tile_count_x; ++i_tiles_x) {
		for (unsigned int i_tiles_y = 0; i_tiles_y < m_tile_count_y; ++i_tiles_y) {
			glm::vec2 tile_pos = glm::vec2(i_tiles_x * m_tile_size, i_tiles_y * m_tile_size);
			for (unsigned int i_node_x = 0; i_node_x < m_tile_resolution; ++i_node_x) {
				for (unsigned int i_node_y = 0; i_node_y < m_tile_resolution; ++i_node_y) {
					std::string id_string = get_node_id(glm::uvec2(i_tiles_x, i_tiles_y), glm::uvec2(i_node_x, i_node_y));
					Node* node = new Node();
					node->id = id_string;
					node->tile_position_in_map = glm::uvec2(i_tiles_x, i_tiles_y);
					node->node_position_in_tile = glm::uvec2(i_node_x, i_node_y);
					float neighbor_distance = (float) m_tile_size / (float) m_tile_resolution;
					float neighbor_distance_diagonal = neighbor_distance * sqrt(2.0f);
					node->position = tile_pos + glm::vec2(i_node_x * neighbor_distance + 1.0f / (2.0f * m_tile_resolution), i_node_y * neighbor_distance + 1.0f / (2.0f * m_tile_resolution));

					// add the existing neighbors
					
					//           N
					//     y
					//     |
					// W   |              E
					//     |_______ x
					//
					//           S
					
					int node_x, node_y, tile_x, tile_y;
					
					// neighbor N
					if (i_node_y == m_tile_resolution - 1) {
						if (i_tiles_y != m_tile_count_y - 1) {
							node->neighbors.push_back(get_node_id(glm::uvec2(i_tiles_x, i_tiles_y + 1), glm::uvec2(i_node_x, 0)));
							node->weights_to_neighbors.push_back(neighbor_distance);
						}
					} else {
						node->neighbors.push_back(get_node_id(glm::uvec2(i_tiles_x, i_tiles_y), glm::uvec2(i_node_x, i_node_y + 1)));
						node->weights_to_neighbors.push_back(neighbor_distance);
					}
					
					// neighbor NW
					node_y = (i_node_y + 1) % m_tile_resolution;
					node_x = (i_node_x - 1) % m_tile_resolution;
					tile_x = i_tiles_x;
					tile_y = i_tiles_y;
					if (i_node_y == m_tile_resolution - 1) {
						tile_y = tile_y + 1;
					}
					if (i_node_x == 0) {
						tile_x = tile_x - 1;
					}
					if (tile_x >= 0 && tile_y < (int) m_tile_count_y) {
						node->neighbors.push_back(get_node_id(glm::uvec2(tile_x, tile_y), glm::uvec2(node_x, node_y)));
						node->weights_to_neighbors.push_back(neighbor_distance_diagonal);
					}
					
					// neighbor W
					if (i_node_x == 0) {
						if (i_tiles_x != 0) {
							node->neighbors.push_back(get_node_id(glm::uvec2(i_tiles_x - 1, i_tiles_y), glm::uvec2(m_tile_resolution - 1, i_node_y)));
							node->weights_to_neighbors.push_back(neighbor_distance);
						}
					} else {
						node->neighbors.push_back(get_node_id(glm::uvec2(i_tiles_x, i_tiles_y), glm::uvec2(i_node_x - 1, i_node_y)));
						node->weights_to_neighbors.push_back(neighbor_distance);
					}
					
					// neighbor SW
					node_y = (i_node_y - 1) % m_tile_resolution;
					node_x = (i_node_x - 1) % m_tile_resolution;
					tile_x = i_tiles_x;
					tile_y = i_tiles_y;
					if (i_node_y == 0) {
						tile_y = tile_y - 1;
					}
					if (i_node_x == 0) {
						tile_x = tile_x - 1;
					}
					if (tile_x >= 0 && tile_y >= 0) {
						node->neighbors.push_back(get_node_id(glm::uvec2(tile_x, tile_y), glm::uvec2(node_x, node_y)));
						node->weights_to_neighbors.push_back(neighbor_distance_diagonal);
					}
					
					// neighbor S
					if (i_node_y == 0) {
						if (i_tiles_y != 0) {
							node->neighbors.push_back(get_node_id(glm::uvec2(i_tiles_x, i_tiles_y - 1), glm::uvec2(i_node_x, m_tile_resolution - 1)));
							node->weights_to_neighbors.push_back(neighbor_distance);
						}
					} else {
						node->neighbors.push_back(get_node_id(glm::uvec2(i_tiles_x, i_tiles_y), glm::uvec2(i_node_x, i_node_y - 1)));
						node->weights_to_neighbors.push_back(neighbor_distance);
					}
					
					// neighbor SE
					node_y = (i_node_y - 1) % m_tile_resolution;
					node_x = (i_node_x + 1) % m_tile_resolution;
					tile_x = i_tiles_x;
					tile_y = i_tiles_y;
					if (i_node_y == 0) {
						tile_y = tile_y - 1;
					}
					if (i_node_x == m_tile_resolution - 1) {
						tile_x = tile_x + 1;
					}
					if (tile_x < (int) m_tile_count_x && tile_y >= 0) {
						node->neighbors.push_back(get_node_id(glm::uvec2(tile_x, tile_y), glm::uvec2(node_x, node_y)));
						node->weights_to_neighbors.push_back(neighbor_distance_diagonal);
					}
					
					// neighbor E
					if (i_node_x == m_tile_resolution - 1) {
						if (i_tiles_x != m_tile_count_x - 1) {
							node->neighbors.push_back(get_node_id(glm::uvec2(i_tiles_x + 1, i_tiles_y), glm::uvec2(0, i_node_y)));
								node->weights_to_neighbors.push_back(neighbor_distance);
						}
					} else {
						node->neighbors.push_back(get_node_id(glm::uvec2(i_tiles_x, i_tiles_y), glm::uvec2(i_node_x + 1, i_node_y)));
						node->weights_to_neighbors.push_back(neighbor_distance);
					}
					
					// neighbor NE
					node_y = (i_node_y + 1) % m_tile_resolution;
					node_x = (i_node_x + 1) % m_tile_resolution;
					tile_x = i_tiles_x;
					tile_y = i_tiles_y;
					if (i_node_y == m_tile_resolution - 1) {
						tile_y = tile_y + 1;
					}
					if (i_node_x == m_tile_resolution - 1) {
						tile_x = tile_x + 1;
					}
					if (tile_x < (int) m_tile_count_x && tile_y < (int) m_tile_count_y) {
						node->neighbors.push_back(get_node_id(glm::uvec2(tile_x, tile_y), glm::uvec2(node_x, node_y)));
						node->weights_to_neighbors.push_back(neighbor_distance_diagonal);
					}
					
					m_nodes[id_string] = node;
				}
			}
		}
	}
}

PathFinder::~PathFinder() {
	for(auto it = m_nodes.begin(); it != m_nodes.end(); ++it) {
		delete it->second;
	}
}

std::list<glm::vec2> PathFinder::get_shortest_path(glm::vec2 start_point, glm::vec2 end_point, bool greedy) {
	std::list<glm::vec2> empty_result;
	// if start == end return empty result
	if (std::abs(start_point.x - end_point.x) < 0.1f && std::abs(start_point.y - end_point.y) < 0.1f) {
		return empty_result;
	}

	// no obstacles on the way. don't use A* in this case
	if (is_walkable(start_point, end_point)) {
		std::list<glm::vec2> result;
		result.push_back(end_point);
		return result;
	}

	// clamp start and end point to get points on the map
	if (start_point.x < 0.0f) {
		start_point.x = 0.0f;
	}
	if (start_point.x >= m_tile_count_x * m_tile_size) {
		start_point.x = (m_tile_count_x - 0.5) * m_tile_size;
	}
	if (start_point.y < 0.0f) {
		start_point.y = 0.0f;
	}
	if (start_point.y >= m_tile_count_y * m_tile_size) {
		start_point.y = (m_tile_count_y - 0.5) * m_tile_size;
	}
	if (end_point.x < 0.0f) {
		end_point.x = 0.0f;
	}
	if (end_point.x >= m_tile_count_x * m_tile_size) {
		end_point.x = (m_tile_count_x - 0.5) * m_tile_size;
	}
	if (end_point.y < 0.0f) {
		end_point.y = 0.0f;
	}
	if (end_point.y >= m_tile_count_y * m_tile_size) {
		end_point.y = (m_tile_count_y - 0.5) * m_tile_size;
	}

	// initialize the data structures
	open_list.clear();
	closed_list.clear();

	bool use_end_node = false;
	std::string start_id = get_nearest_walkable_node(start_point);
	std::string end_id = get_nearest_node(end_point);
	if (end_id == "-1") {
		end_id = get_nearest_walkable_node(end_point);
		if (end_id == "-1") {
			return empty_result;
		}
		use_end_node = true;
	}
	if (start_id == "-1" || end_id == "-1") {
		return empty_result;
	}
	Node start_node = *m_nodes[start_id];
	Node end_node = *m_nodes[end_id];
	start_node.previous_node = "none";
	start_node.bee_line = glm::length(end_node.position - start_node.position);
	start_node.key = start_node.bee_line;
	start_node.already_walked = 0.0f;
	closed_list[start_node.id] = start_node;
	
	Node last_node = start_node;
	add_neighbors_to_open_list(start_node, last_node, end_node.position, greedy);
	bool success = false;
	Node final_node;

	// start the A* algorithm
	while (open_list.size() > 0) {
		// take the node which has currently the smallset estimated 
		// distance to the destiny (already_walked + bee_line)
		// since the open_list is ordered ascending, we just have to take the front
		Node current_node = open_list.front();
		// put the node in the closed_list
		delete_node_in_list(current_node);
		closed_list[current_node.id] = current_node;

		// destiny was found => termination
		if (current_node.id == end_node.id) {
			final_node = current_node;
			success = true;
			break;
		}

		// handle all the neighbors of the current node by adding
		// them to the open_list
		add_neighbors_to_open_list(current_node, last_node, end_node.position, greedy);
		last_node = current_node;
	}

	if (success) {
		std::list<glm::vec2> result;

		// create a list which contains all node positions on the way from start to end
		// this reconstruction can be done by walking through the nodes
		// by looking at their previous nodes
		Node current_node = final_node;
		bool first = true;
		while (current_node.id != start_node.id) {
			if (first) {
				first = false;
			} else {
				result.push_front(current_node.position);
			}
			current_node = closed_list[current_node.previous_node];
		}
		// don't forget the start node, the start point and end point
		result.push_front(start_node.position);
		result.push_front(start_point);
		if (!use_end_node) {
			result.push_back(end_point);
		}
		
		// optimize result
		std::vector<glm::vec2> result_vector;
		for (auto it = result.begin(); it != result.end(); ++it) {
			result_vector.push_back(*it);
		}
		
		if (result_vector.size() > 2) {
			glm::vec2 end1 = result_vector[result_vector.size() - 3] - result_vector[result_vector.size() - 1];
			glm::vec2 end2 = result_vector[result_vector.size() - 2] - result_vector[result_vector.size() - 1];
			if (glm::length(end1) <= glm::length (end2)) {
				result_vector.erase(result_vector.begin() + result_vector.size() - 2);
			}
		}
		bool start_node_flag = false;
		if (result_vector.size() > 2) {
			glm::vec2 start1 = result_vector[2] - result_vector[0];
			glm::vec2 start2 = result_vector[1] - result_vector[0];
			if (glm::length(start1) <= glm::length (start2)) {
				result_vector.erase(result_vector.begin() + 1);
				start_node_flag = true;
			}
		}
		if (result_vector.size() > 2) {
			glm::vec2 start1 = result_vector[2] - result_vector[0];
			glm::vec2 start2 = result_vector[1] - result_vector[0];
			if (glm::length(start1) <= glm::length (start2)) {
				result_vector.erase(result_vector.begin() + 1);
			}
		}
		
		if (result_vector.size() > 2) {
			for (unsigned int i = 1; i < result_vector.size() - 1; ++i) {
				glm::vec2 dir1 = glm::normalize(result_vector[i] - result_vector[i - 1]);
				glm::vec2 dir2 = glm::normalize(result_vector[i + 1] - result_vector[i]);

				if ((std::abs(dir1.x - dir2.x) < 0.0001f) && (std::abs(dir1.y - dir2.y) < 0.0001f)) {
					result_vector.erase(result_vector.begin() + i);
					i--;
				}
			}
		}
		
		if (result_vector.size() > 2) {
			unsigned int i = 0;
			while (i < result_vector.size() - 2) {
				if (is_walkable(result_vector[i], result_vector[i + 2])) {
					result_vector.erase(result_vector.begin() + i + 1);
				} else {
					i++;
				}
			}
		}

		/*if (is_walkable(result_vector[0], result_vector[result_vector.size() - 1])) {
			glm::vec2 destiny = result_vector[result_vector.size() - 1];
			result_vector.clear();
			result_vector.push_back(destiny);
		} else {*/
		result_vector.erase(result_vector.begin());
		if (!start_node_flag) {
			result_vector.erase(result_vector.begin());
		}		
		//}
		
		std::list<glm::vec2> optimized_result;
		for (unsigned int i = 0; i < result_vector.size(); ++i) {
			optimized_result.push_back(result_vector[i]);
		}
		
		return optimized_result;
	} else {
		// if the destiny was not reached return empty list
		return empty_result;
	}
	
}

std::string PathFinder::get_node_id(glm::uvec2 tile_coordinates, glm::uvec2 node_coordinates) {
	std::string comma = ",";
	std::string id = std::to_string(tile_coordinates.x) + comma + std::to_string(tile_coordinates.y) + comma + std::to_string(node_coordinates.x) + comma + std::to_string(node_coordinates.y);
	return id;
}

std::string PathFinder::get_nearest_node(glm::vec2 point) {
	float neighbor_distance = (float) m_tile_size / (float) m_tile_resolution;

	unsigned int tile_x = point.x / m_tile_size;
	unsigned int tile_y = point.y / m_tile_size;
	unsigned int node_x = (point.x - tile_x) / neighbor_distance;
	unsigned int node_y = (point.y - tile_y) / neighbor_distance;
	std::string id = get_node_id(glm::uvec2(tile_x, tile_y), glm::uvec2(node_x, node_y));
	Tile& nearest_tile = m_map.get_tile(tile_x, tile_y);
	if (nearest_tile.is_walkable()) {
		return id;
	} else {
		return "-1";
	}
}

std::string PathFinder::get_nearest_walkable_node(glm::vec2 point) {
	float neighbor_distance = (float) m_tile_size / (float) m_tile_resolution;

	unsigned int tile_x = point.x / m_tile_size;
	unsigned int tile_y = point.y / m_tile_size;
	unsigned int node_x = (point.x - tile_x) / neighbor_distance;
	unsigned int node_y = (point.y - tile_y) / neighbor_distance;

	std::string id = get_node_id(glm::uvec2(tile_x, tile_y), glm::uvec2(node_x, node_y));
	Tile& nearest_tile = m_map.get_tile(tile_x, tile_y);
	
	if (nearest_tile.is_walkable()) {
		return id;
	} else {
		float best_distance = m_tile_size * sqrt(2) * (std::max(m_tile_count_x, m_tile_count_y)) / 2.0f;
		unsigned int best_x = 0;
		unsigned int best_y = 0;
		bool better_tile_found = false;
		
		for (int i_step = 1; i_step < (std::max(m_tile_count_x, m_tile_count_y)) / 2.0f; ++i_step) {
			for (int i_x = -i_step; i_x <= i_step; ++i_x) {
				for (int i_y = -i_step; i_y <= i_step; ++i_y) {
					if (std::abs(i_x) == i_step || std::abs(i_y) == i_step) {
						int current_tile_x = tile_x + i_x;
						int current_tile_y = tile_y + i_y;
						if (current_tile_x < (int) m_tile_count_x && current_tile_x >= 0 && current_tile_y < (int) m_tile_count_y && current_tile_y >= 0) {
							Tile& current_tile = m_map.get_tile(current_tile_x, current_tile_y);
							if (current_tile.is_walkable()) {
								glm::vec2 current_tile_midpoint = glm::vec2((current_tile_x + 0.5f) * (float) m_tile_size, (current_tile_y + 0.5f) * (float) m_tile_size);
								float current_distance_to_point = glm::length(point - current_tile_midpoint);
								if (current_distance_to_point < best_distance) {
									best_distance = current_distance_to_point;
									best_x = current_tile_x;
									best_y = current_tile_y;
									better_tile_found = true;
								}
							}
						}
					}
				}
			}
			if (better_tile_found) {
				break;
			}
		}
		if (!better_tile_found) {
			return "-1";
		}
		
		unsigned int final_node_x = node_x;
		unsigned int final_node_y = node_y;
		if (best_x > tile_x) {
			final_node_x = 0;
		} else if (best_x < tile_x) {
			final_node_x = m_tile_resolution - 1;
		}
		if (best_y > tile_y) {
			final_node_y = 0;
		} else if (best_y < tile_y) {
			final_node_y = m_tile_resolution - 1;
		}
		std::string final_id = get_node_id(glm::uvec2(best_x, best_y), glm::uvec2(final_node_x, final_node_y));
		return final_id;
	}
}

void PathFinder::insert_into_open_list(Node node, bool greedy) {
	// only insert in the open list if the node is not in the closed list
	if (closed_list.find(node.id) != closed_list.end()) {
		// just to be sure
		delete_node_in_list(node);
		return;
	}
	
	if(open_list.size() == 0)
	{
		open_list.push_back(node);
		return;
	}
	
	// look if the node is already in the open list
	for(auto it = open_list.begin(); it != open_list.end(); ++it)
	{
		// if the node is already in the list AND its key has
		// become better, update
		if(it->id == node.id)
		{
			if (!greedy && (it->key > node.key)) {
				delete_node_in_list(*it);
				insert_into_open_list(node, greedy);
				return;
			} else if (greedy && (it->bee_line > node.bee_line)) {
				delete_node_in_list(*it);
				insert_into_open_list(node, greedy);
				return;
			// else: do nothing
			} else {
				return;
			}
		}
	}

	// the node is not already in the open list
	for(auto it = open_list.begin(); it != open_list.end(); ++it)
	{
		// insert at the correct position
		// (the open list must be ordered)
		if((greedy && (it->bee_line > node.bee_line)) || (!greedy && (it->key > node.key)))
		{
			open_list.insert(it, node);
			return;
		}
	}
	open_list.push_back(node);
}

// deletes a node in the open_list if the open_list already contains it
bool PathFinder::delete_node_in_list(Node node) {
	for(auto it = open_list.begin(); it != open_list.end(); ++it)
	{
		if(it->id == node.id)
		{
			open_list.erase(it);
			return true;
		}
	}
	return false;
}

void PathFinder::add_neighbors_to_open_list(Node node, Node last_node, glm::vec2 end_node_position, bool greedy) {
	for(unsigned int i = 0; i < node.neighbors.size(); ++i) {
		// store some information in the current node
		Node current_node = *m_nodes[node.neighbors[i]];
		
		// check whether the current node is walkable
		Tile& current_tile = m_map.get_tile(current_node.tile_position_in_map.x, current_node.tile_position_in_map.y);
		if (current_tile.is_walkable()) {
			current_node.previous_node = node.id;
			current_node.bee_line = glm::length(current_node.position - end_node_position);
			current_node.already_walked = node.weights_to_neighbors[i] + node.already_walked;
			if (node.id == last_node.id) {
				current_node.key = current_node.already_walked + current_node.bee_line;
			} else {
				glm::vec2 dir1 = glm::normalize(node.position - last_node.position);
				glm::vec2 dir2 = glm::normalize(current_node.position - node.position);
				float angle = acos(glm::dot(dir1, dir2));
				
				float penalty = (angle / 3.1415f) * 2.0f * m_tile_size;
				current_node.key = current_node.already_walked + current_node.bee_line + penalty;
			}
			// and try to insert it into the openList
			insert_into_open_list(current_node, greedy);
		}
	}
}

float PathFinder::safe_acos (float x) {
  if (x < - 1.0f) x = -1.0f ;
  else if (x > 1.0f) x = 1.0f ;
  return acos(x) ;
}

bool PathFinder::is_walkable(glm::vec2 start, glm::vec2 end) {
	float slope = (end.y - start.y) / (end.x - start.x);
	unsigned int steps = std::abs(end.x - start.x) / m_tile_size;
	float current_x = std::min(start.x, end.x);
	float current_y = (start.x < end.x) ? start.y : end.y;
	
	if (steps == 0) {
		float real_end_y = (start.x < end.x) ? end.y : start.y;
		if (slope > 0.0f) {
			while (current_y < static_cast<float>(m_tile_count_y) && current_y < real_end_y) {
				Tile& current_tile = m_map.get_tile((int) current_x, (int) current_y);
				if (!current_tile.is_walkable()) {
					return false;
				}
				current_y += m_tile_size;
			}
		} else {
			while ((int) current_y >= 0.0 && current_y > real_end_y) {
				Tile& current_tile = m_map.get_tile((int) current_x, (int) current_y);
				if (!current_tile.is_walkable()) {
					return false;
				}
				current_y -= m_tile_size;
			}
		}
	}
	current_x = std::min(start.x, end.x) + m_tile_size;
	current_y = (start.x < end.x) ? start.y : end.y;

	for (unsigned int i = 0; i <= steps; ++i) {
		if (current_x >= static_cast<float>(m_tile_count_x) || current_y >= static_cast<float>(m_tile_count_y)) {
			return false;
		}
		
		float y = current_y;
		if (slope > 0.0f) {
			while (y < static_cast<float>(m_tile_count_y) && y < current_y + m_tile_size * slope) {
				Tile& current_tile = m_map.get_tile((int) current_x, (int) y);
				if (!current_tile.is_walkable()) {
					return false;
				}
				Tile& current_tile_left = m_map.get_tile((int) current_x - 1, (int) y);
				if (!current_tile_left.is_walkable()) {
					return false;
				}
				y += m_tile_size;
			}
		} else {
			while ((int) y >= 0.0 && y > current_y + m_tile_size * slope) {
				Tile& current_tile = m_map.get_tile((int) current_x, (int) y);
				if (!current_tile.is_walkable()) {
					return false;
				}
				Tile& current_tile_left = m_map.get_tile((int) current_x - 1, (int) y);
				if (!current_tile_left.is_walkable()) {
					return false;
				}
				y -= m_tile_size;
			}
		}

		current_x += m_tile_size;
		current_y += m_tile_size * slope;
	}
	return true;
}
