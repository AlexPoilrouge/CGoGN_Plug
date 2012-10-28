#include "tuto5Topo.h"

#include <iostream>

#include "Algo/Modelisation/primitives3d.h"
#include "Algo/Modelisation/polyhedron.h"
#include "Algo/Modelisation/subdivision.h"
#include "Algo/Modelisation/subdivision3.h"

#include "Algo/Render/GL2/topo3Render.h"
#include "Algo/Render/SVG/mapSVGRender.h"

Dart dglobal;


Tuto5Topo::Tuto5Topo() :
	myMap(NULL),
	m_vizuHandler(NULL),
	m_render_topo(NULL),
	m_positionVBO(NULL),
	m_selected(NIL),
	m_transfo_matrix(m_mat.m_matrices[2])
{
	m_transfo_matrix = glm::mat4(1.0f);
}

bool Tuto5Topo::activate(){
	waitingForScene(1);
//#ifndef DEBUG
//	Plugin* dep=this->getDependencie<Plugin>("t5_part1");
//#else
//	Plugin* dep= this->getDependencie<Plugin>("t5_part1_D");
//#endif
//	if(!dep){
//		System::Error::showError();
//		return false;
//	}

	m_vizuHandler= this->getReferencedMap("T5_Map");
	if(!m_vizuHandler){
		System::Error::showError();

		return false;
	}
	myMap= (MAP*)m_vizuHandler->map();

	position= myMap->getAttribute<PFP::VEC3, VERTEX>("position");

	m_selected= myMap->begin();

//	if(!this->associateGLViewer("t5Parts_glv",m_glviewer,true)){
//		System::Error::showError(window);
//		return false;
//	}
//
//	std::cout << "This plugin (" << name.toStdString() <<  ") is active!" << std::endl;



	GLint texSize;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texSize);
	return true;
}


void Tuto5Topo::cb_updateMatrix(View* view)
{
	if(view){
		glm::mat4 model(view->getCurrentModelViewMatrice());
		glm::mat4 proj(view->getCurrentProjectionMatrice());

		model *= m_transfo_matrix;


		for(std::set< std::pair<void*, Utils::GLSLShader*> >::iterator it = Utils::GLSLShader::m_registeredShaders.begin();
			it != Utils::GLSLShader::m_registeredShaders.end();
			++it)
		{
			if ((it->first == NULL) || (it->first == this))
			{
				it->second->updateMatrices(proj, model);
			}
		}
	}
}

void Tuto5Topo::disable(){
	this->unloadDependantPlugins();
	this->suppressAllDependencieLinks();

	m_selected=NIL;

	if(m_render_topo)
		delete m_render_topo;

}

void Tuto5Topo::cb_initGL(Scene* scene){
		m_render_topo = new Algo::Render::GL2::Topo3Render();


		SelectorDartNoBoundary<PFP::MAP> nb(*myMap);
		m_render_topo->updateData<PFP>(*myMap, position,  0.9f, 0.9f, 0.9f, nb);

}

void Tuto5Topo::cb_redraw(Scene* scene){
//	if(qglv==m_glviewer){
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.0f, 1.0f);

		m_render_topo->drawTopo();

		m_render_topo->overdrawDart(m_selected, 5, 1.0f, 0.0f, 1.0f);

		glDisable(GL_POLYGON_OFFSET_FILL);

//	}
}



#ifndef DEBUG
	Q_EXPORT_PLUGIN2(Tuto5Topo, Tuto5Topo)
#else
	Q_EXPORT_PLUGIN2(Tuto5TopoD, Tuto5Topo)
#endif

