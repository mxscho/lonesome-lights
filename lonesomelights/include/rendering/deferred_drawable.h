#ifndef __RENDERING__DEFERRED_DRAWABLE_H__
#define __RENDERING__DEFERRED_DRAWABLE_H__

#include "rendering/drawable.h"

class Camera;
class RenderProgram;
class Texture;

class DeferredDrawable : public Drawable {
public:
	DeferredDrawable(const RenderProgram& render_program, const RenderProgram& deferred_render_program);

	virtual void draw_deferred(const Camera& camera, const Texture& color_texture, const Texture& position_texture, const Texture& normal_texture, const Texture& depth_texture) const = 0;
protected:
	const RenderProgram& m_deferred_render_program;
};

#endif