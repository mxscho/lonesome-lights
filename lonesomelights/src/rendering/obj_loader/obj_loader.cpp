#include "rendering/obj_loader/obj_loader.h"

const std::vector<GLfloat>& ObjLoader::get_obj_positions(const std::string& name, unsigned int shape_index) {
	ObjLoader::get_obj(name);
	return m_obj_shapes.find(name)->second[shape_index].mesh.positions;
}
const std::vector<GLfloat>& ObjLoader::get_obj_normals(const std::string& name, unsigned int shape_index) {
	ObjLoader::get_obj(name);
	return m_obj_shapes.find(name)->second[shape_index].mesh.normals;
}
const std::vector<GLuint>& ObjLoader::get_obj_elements(const std::string& name, unsigned int shape_index) {
	ObjLoader::get_obj(name);
	return m_obj_shapes.find(name)->second[shape_index].mesh.indices;
}

void ObjLoader::get_obj(const std::string& name) {
	if (ObjLoader::m_obj_shapes.find(name) == ObjLoader::m_obj_shapes.end()) {
		std::string obj_file_path = "res/models/" + name + ".obj";
		std::string mtl_file_path = "res/models/" + name + ".mtl";
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		tinyobj::LoadObj(shapes, materials, obj_file_path.c_str(), mtl_file_path.c_str());
		ObjLoader::m_obj_shapes.insert(std::make_pair(name, shapes));
	}
}

std::map<std::string, std::vector<tinyobj::shape_t>> ObjLoader::m_obj_shapes = std::map<std::string, std::vector<tinyobj::shape_t>>();