#include "geometry/path_finder.h"


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
			for (unsigned int i_node_x = 0; i_node_x < tile_resolution; ++i_node_x) {
				for (unsigned int i_node_y = 0; i_node_y < tile_resolution; ++i_node_y) {
					std::string id_string = get_node_id(glm::uvec2(i_tiles_x, i_tiles_y), glm::uvec2(i_node_x, i_node_y));
					Node* node = new Node();
					node->id = id_string;
					node->tile_position_in_map = glm::uvec2(i_tiles_x, i_tiles_y);
					node->node_position_in_tile = glm::uvec2(i_node_x, i_node_y);
					float neighbor_distance = (float) m_tile_size / (float) m_tile_resolution;
					float neighbor_distance_diagonal = neighbor_distance * sqrt(2.0f);
					node->position = tile_pos + glm::vec2(i_node_x * neighbor_distance, i_node_y * neighbor_distance);

					// add the existing neighbors
					
					//           N
					//     y
					//     |
					// W   |              E
					//     |_______ x
					//
					//           S
					
					unsigned int node_x, node_y, tile_x, tile_y;
					
					// neighbor N
					if (i_node_y == tile_resolution - 1) {
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
					node_x = (i_node_x - 1) & m_tile_resolution;
					tile_x = i_tiles_x;
					tile_y = i_tiles_y;
					if (i_node_y == m_tile_resolution) {
						tile_y = tile_y + 1;
					}
					if (i_node_x == 0) {
						tile_x = tile_x - 1;
					}
					if (tile_x >= 0 && tile_y < m_tile_count_y) {
						node->neighbors.push_back(get_node_id(glm::uvec2(tile_x, tile_y), glm::uvec2(node_x, node_y)));
						node->weights_to_neighbors.push_back(neighbor_distance_diagonal);
					}
					
					// neighbor W
					if (i_node_x == 0) {
						if (i_tiles_x != 0) {
							node->neighbors.push_back(get_node_id(glm::uvec2(i_tiles_x - 1, i_tiles_y), glm::uvec2(tile_resolution - 1, i_node_y)));
							node->weights_to_neighbors.push_back(neighbor_distance);
						}
					} else {
						node->neighbors.push_back(get_node_id(glm::uvec2(i_tiles_x, i_tiles_y), glm::uvec2(i_node_x - 1, i_node_y)));
						node->weights_to_neighbors.push_back(neighbor_distance);
					}
					
					// neighbor SW
					node_y = (i_node_y - 1) % m_tile_resolution;
					node_x = (i_node_x - 1) & m_tile_resolution;
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
							node->neighbors.push_back(get_node_id(glm::uvec2(i_tiles_x, i_tiles_y - 1), glm::uvec2(i_node_x, tile_resolution - 1)));
							node->weights_to_neighbors.push_back(neighbor_distance);
						}
					} else {
						node->neighbors.push_back(get_node_id(glm::uvec2(i_tiles_x, i_tiles_y), glm::uvec2(i_node_x, i_node_y + 1)));
						node->weights_to_neighbors.push_back(neighbor_distance);
					}
					
					// neighbor SE
					node_y = (i_node_y - 1) % m_tile_resolution;
					node_x = (i_node_x + 1) & m_tile_resolution;
					tile_x = i_tiles_x;
					tile_y = i_tiles_y;
					if (i_node_y == 0) {
						tile_y = tile_y - 1;
					}
					if (i_node_x == m_tile_resolution) {
						tile_x = tile_x + 1;
					}
					if (tile_x < m_tile_count_x && tile_y >= 0) {
						node->neighbors.push_back(get_node_id(glm::uvec2(tile_x, tile_y), glm::uvec2(node_x, node_y)));
						node->weights_to_neighbors.push_back(neighbor_distance_diagonal);
					}
					
					// neighbor E
					if (i_node_x == tile_resolution - 1) {
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
					node_x = (i_node_x + 1) & m_tile_resolution;
					tile_x = i_tiles_x;
					tile_y = i_tiles_y;
					if (i_node_y == 0) {
						tile_y = tile_y + 1;
					}
					if (i_node_x == m_tile_resolution) {
						tile_x = tile_x + 1;
					}
					if (tile_x < m_tile_count_x && tile_y < m_tile_count_y) {
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

	// initialize the data structures
	open_list.clear();
	closed_list.clear();
	
	Node start_node = *m_nodes[get_nearest_node(start_point)];
	Node end_node = *m_nodes[get_nearest_node(end_point)];
	start_node.previous_node = "none";
	start_node.bee_line = glm::length(end_node.position - start_node.position);
	start_node.key = start_node.bee_line;
	start_node.already_walked = 0.0f;
	closed_list[start_node.id] = start_node;
	
	add_neighbors_to_open_list(start_node, end_node.position, greedy);

	bool success = false;
	Node last;
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
			last = current_node;
			success = true;
			break;
		}

		// handle all the neighbors of the current node by adding
		// them to the open_list
		add_neighbors_to_open_list(current_node, end_node.position, greedy);
	}


	if (success) {
		std::list<glm::vec2> result;

		// create a list which contains all node positions on the way from start to end
		// this reconstruction can be done by walking through the nodes
		// by looking at their previous nodes
		Node current_node = last;
		while (current_node.id != start_node.id) {
			result.push_front(current_node.position);
			current_node = closed_list[current_node.previous_node];
		}
		// don't forget the start node, the start point and end point
		result.push_front(start_node.position);
		result.push_front(start_point);
		result.push_back(end_point);
		
		return result;
	} else {
		std::list<glm::vec2> empty_result;
		// if the destiny was not reached return empty vector
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
	unsigned int node_y = (point.x - tile_y) / neighbor_distance;
	std::string id = get_node_id(glm::uvec2(tile_x, tile_y), glm::uvec2(node_x, node_y));
	return id;
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

void PathFinder::add_neighbors_to_open_list(Node node, glm::vec2 end_node_position, bool greedy) {
	for(unsigned int i = 0; i < node.neighbors.size(); ++i) {
		// store some information in the current node
		Node current_node = *m_nodes[node.neighbors[i]];
		current_node.previous_node = node.id;
		current_node.bee_line = glm::length(current_node.position - end_node_position);
		current_node.already_walked = node.weights_to_neighbors[i] + node.already_walked;
		current_node.key = current_node.already_walked + current_node.bee_line;
		// and try to insert it into the openList
		insert_into_open_list(current_node, greedy);
	}
}