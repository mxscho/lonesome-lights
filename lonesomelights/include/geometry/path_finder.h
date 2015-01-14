#ifndef __GEOMETRY__PATH_FINDER_H__
#define __GEOMETRY__PATH_FINDER_H__

#include "game/map/map.h"
#include "game/map/tile.h"

#include <glm/glm.hpp>
#include <list>
#include <string>
#include <unordered_map>

struct Node{
	std::string id;
	std::string previous_node = "none";
	glm::uvec2 tile_position_in_map;
	glm::uvec2 node_position_in_tile;
	std::vector<std::string> neighbors;
	std::vector<float> weights_to_neighbors;
	glm::vec2 position;
	float key;
	float bee_line;
	float already_walked;
};

class PathFinder{
public:
	PathFinder(Map& map, unsigned int tile_resolution);
	~PathFinder();
	std::list<glm::vec2> get_shortest_path(glm::vec2 start_point, glm::vec2 end_point, bool greedy);
	
private:
	Map& m_map;
	unsigned int m_tile_resolution;
	float m_tile_size;
	unsigned int m_tile_count_x;
	unsigned int m_tile_count_y;
	
	std::unordered_map<std::string, Node*> m_nodes;
	std::vector<Node> open_list;
	std::unordered_map<std::string, Node> closed_list;
	
	std::string get_node_id(glm::uvec2 tile_coordinates, glm::uvec2 node_coordinates);
	std::string get_nearest_node(glm::vec2 point);
	void insert_into_open_list(Node node, bool greedy);
	bool delete_node_in_list(Node node);
	void add_neighbors_to_open_list(Node node, glm::vec2 end_node_position, bool greedy);
	
};
#endif