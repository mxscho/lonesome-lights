#ifndef __RENDERING__DRAWABLE_H__
#define __RENDERING__DRAWABLE_H__

class Camera;
class RenderProgram;

class Drawable {
public:
	Drawable(const RenderProgram& render_program);

	virtual void draw(const Camera& camera) const = 0;
protected:
	const RenderProgram& m_render_program;
};

#endif