#ifndef __RENDERING__OPENGL__TEXTURES_H__
#define __RENDERING__OPENGL__TEXTURES_H__

#include "rendering/opengl/texture.h"

#include <string>
#include <map>

class Textures {
public:
	static const Texture& get_texture(const std::string& name);
private:
	static std::map<std::string, Texture> m_textures;
};

#endif