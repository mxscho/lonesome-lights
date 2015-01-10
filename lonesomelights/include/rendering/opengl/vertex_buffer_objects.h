#ifndef __RENDERING__OPENGL__VERTEX_BUFFER_OBJECTS_H__
#define __RENDERING__OPENGL__VERTEX_BUFFER_OBJECTS_H__

#include <GL/glew.h>

class VertexBufferObjects {
public:
	static void unbind_any_from(GLenum target);
	static void unbind_any();
};

#endif