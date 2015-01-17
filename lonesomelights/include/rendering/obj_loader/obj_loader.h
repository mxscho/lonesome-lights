#ifndef __RENDERING__OBJ_LOADER__OBJ_LOADER_H__
#define __RENDERING__OBJ_LOADER__OBJ_LOADER_H__

#include "rendering/obj_loader/tiny_obj_loader.h"

#include <string>
#include <map>
#include <vector>
#include <GL/glew.h>

class ObjLoader {
public:
	static const std::vector<GLfloat>& get_obj_positions(const std::string& name, unsigned int shape_index);
	static const std::vector<GLfloat>& get_obj_normals(const std::string& name, unsigned int shape_index);
	static const std::vector<GLuint>& get_obj_elements(const std::string& name, unsigned int shape_index);
private:
	static void get_obj(const std::string& name);
	
	static std::map<std::string, std::vector<tinyobj::shape_t>> m_obj_shapes;
};

#endif