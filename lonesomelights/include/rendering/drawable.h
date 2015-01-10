#ifndef __RENDERING__DRAWABLE_H__
#define __RENDERING__DRAWABLE_H__

class RenderProgram;

class Drawable {
public:
	virtual void draw(const RenderProgram& render_program) const = 0;
};

#endif