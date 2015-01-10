#include "rendering/opengl/vertex_buffer_objects.h"

void VertexBufferObjects::unbind_any_from(GLenum target) {
	glBindBuffer(target, 0);
}
void VertexBufferObjects::unbind_any() {
	VertexBufferObjects::unbind_any_from(GL_ARRAY_BUFFER);
	VertexBufferObjects::unbind_any_from(GL_ELEMENT_ARRAY_BUFFER);
	// TODO: Add further targets.
}