#include "rendering/opengl/textures.h"

#include <utility>

const Texture& Textures::get_texture(const std::string& name) {
	if (Textures::m_textures.find(name) == Textures::m_textures.end()) {
		Texture texture;
		texture.load_from_file(std::string("res/textures/") + name + std::string(".png"), GL_TEXTURE0);
		Textures::m_textures.insert(std::make_pair(name, std::move(texture)));
	}
	return Textures::m_textures.find(name)->second;
}

std::map<std::string, Texture> Textures::m_textures = std::map<std::string, Texture>();