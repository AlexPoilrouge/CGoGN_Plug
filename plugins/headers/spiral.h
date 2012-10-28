#ifndef _SPIRAL_H_
#define _SPIRAL_H_

#include "visualPlugin.h"

class Spiral : public VisualPlugin{
	Q_OBJECT
	Q_INTERFACES(Plugin)
public:
	Spiral(){}
	~Spiral(){}

	void cb_updateMatrix(View* view){}

	void cb_redraw(Scene* scene);

	void cb_initGL(Scene* scene);

protected:
	Scene* my_scene;


	bool activate();
	void disable();
};


#endif
