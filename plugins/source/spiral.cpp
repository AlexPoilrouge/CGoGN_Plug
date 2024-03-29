#include "spiral.h"


void Spiral::cb_redraw(Scene* scene){
	std::cout << "spiral redraw" << std::endl;
	const float nbSteps= 200.0;
	glBegin(GL_QUAD_STRIP);
	for (float i=0; i<nbSteps; ++i){
		float ratio= i/nbSteps;
		float angle= 21.0*ratio;
		float c= cos(angle);
		float s= sin(angle);
		float r1= 1.0 - 0.8*ratio;
		float r2= 0.8 - 0.8*ratio;
		float alt= ratio - 0.5;
		const float nor= .5;
		const float up= sqrt(1.0-nor-nor);
		glColor3f(1.0-ratio, 0.2f, ratio);
		glNormal3f(nor*c, up, nor*s);
		glVertex3f(r1*c, alt, r1*s);
		glVertex3f(r2*c, alt+0.05, r2*s);
	}
	glEnd();

}

void Spiral::cb_initGL(Scene* scene){
	std::cout << "In init?" << std::endl;
}

bool Spiral::activate(){
//	this->addNewGLViewer("1stGLV",my_glv);
	this->addNewSceneDialog("1stGLV",my_scene);
	if(!my_scene){
		System::Error::showError(m_window);
		return false;
	}

	return System::Error::code==System::Error::SUCCESS;
}

void Spiral::disable(){
	if(my_scene){
		my_scene->suppressLinkWith(this);
	}
}

#ifndef DEBUG
	Q_EXPORT_PLUGIN2(Spiral, Spiral)
#else
	Q_EXPORT_PLUGIN2(SpiralD, Spiral)
#endif
