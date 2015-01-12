#ifndef __RENDERING__DRAWABLE_H__
#define __RENDERING__DRAWABLE_H__

class Camera;
class RenderProgram;

class Drawable {
public:
	virtual void draw(const RenderProgram& render_program, const Camera& camera) const = 0;
protected:
	void prepare_draw(const RenderProgram& render_program, const Camera& camera) const;
	void finalize_draw(const RenderProgram& render_program) const;
};

#endif