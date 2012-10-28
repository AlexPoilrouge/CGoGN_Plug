#include "tuto5Geom.h"


#include <iostream>

#include "Algo/Modelisation/primitives3d.h"
#include "Algo/Modelisation/polyhedron.h"
#include "Algo/Modelisation/subdivision.h"
#include "Algo/Modelisation/subdivision3.h"

#include "Algo/Render/GL2/topo3Render.h"
#include "Algo/Render/SVG/mapSVGRender.h"


Dart dglobal;

Tuto5Geom::Tuto5Geom() :
	myMap(NULL),
	m_vizuHandler(NULL),
	m_scene(NULL),
	m_transfo_matrix(m_mat.m_matrices[2]),
	m_timer(NULL),
	m_selected(NIL),
	m_render(NULL),
	m_shader(NULL),
	m_positionVBO(NULL)
{
	m_transfo_matrix = glm::mat4(1.0f);
	m_init=false;
}

Tuto5Geom::~Tuto5Geom(){}

void Tuto5Geom::animate()
{
	transfoRotate( 0.5f, 0.5773f,0.5773f,0.5773f);
//	m_scene->updateGL();
	foreach(Scene* scene, l_recievedScene){
		scene->updateGL();
	}
}

void Tuto5Geom::transfoRotate(float angle, float x, float y, float z)
{
	m_transfo_matrix = glm::rotate(m_transfo_matrix, angle, glm::vec3(x,y,z));
}

bool Tuto5Geom::cb_keyPress(Scene* scene, int event){
		if (event  == 'T')
		{
			if (m_timer->isActive())
				m_timer->stop();
			else
				m_timer->start(1000/30); // 30 fps
			return true;
		}

		return false;
}

bool Tuto5Geom::activate(){
	waitingForScene(2);

	if(!(m_vizuHandler=this->addNewReferencedMap<MAP>("T5_Map",myMap))){
		System::Error::showError();
		return false;
	}

	if(!position.isValid()){
		position = myMap->addAttribute<PFP::VEC3, VERTEX>("position");

		Algo::Modelisation::Primitive3D<PFP> prim(*myMap, position);
		int nb=3;
		dglobal = prim.hexaGrid_topo(nb,nb,nb);
		prim.embedHexaGrid(1.0f,1.0f,1.0f);
	}

	m_selected= myMap->begin();


	GLint texSize;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texSize);

	m_timer = new QTimer( this );
	connect( m_timer, SIGNAL(timeout()), this, SLOT(animate()) );

	return true;
}

void Tuto5Geom::cb_initGL(Scene* scene){
	std::cout << "INITGL" << std::endl;
	if(scene /*&& !m_init*/){

		std::cout << "hey!!" << std::endl;

		if(!m_render){
			m_render= new Algo::Render::GL2::MapRender();
		}
		VBOHandler* vbo;
		if((vbo=m_vizuHandler->findVBO("positionVBO"))){
			m_positionVBO= vbo;
		}
		else{
			m_positionVBO = new VBOHandler("positionVBO");
			m_vizuHandler->addVBO(m_positionVBO);
		}
		// using simple shader with color
		if(!m_shader){
			m_shader = new Utils::ShaderSimpleColor();
		}
			m_shader->setAttributePosition(m_positionVBO);
			m_shader->setColor(Geom::Vec4f(0.,1.,0.,0.));

			Utils::GLSLShader::registerShader(NULL,m_shader);


	//		m_positionVBO = new Utils::VBO();
		m_positionVBO->updateData(position);

			m_render->initPrimitives<PFP>(*myMap, allDarts, Algo::Render::GL2::TRIANGLES);
			m_render->initPrimitives<PFP>(*myMap, allDarts, Algo::Render::GL2::LINES);
			m_render->initPrimitives<PFP>(*myMap, allDarts, Algo::Render::GL2::POINTS);

		//SelectorTrue allDarts;

		m_init= true;
	}

}

void Tuto5Geom::cb_recievedMap(MapHandler* map){
	std::cout << "Map recieved" << std::endl;
}

void Tuto5Geom::cb_removingMap(MapHandler* map){
	std::cout << "Map removed" << std::endl;
}

void Tuto5Geom::disable(){
	if(m_timer){
		delete m_timer;
	}
	if(m_shader){
		Utils::GLSLShader::unregisterShader(NULL,m_shader);
		delete m_shader;
	}
	if(m_render){
		delete m_render;
	}
}

void Tuto5Geom::cb_redraw(Scene* scene)
{
	if(scene){
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_LIGHTING);

		m_render->draw(m_shader, Algo::Render::GL2::POINTS);

		glLineWidth(2.0f);
		m_shader->setColor(Geom::Vec4f(1.,1.,0.,0.));
		m_render->draw(m_shader, Algo::Render::GL2::LINES);

		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.0f, 1.0f);
	}
}

void Tuto5Geom::cb_updateMatrix(View* view)
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


#ifndef DEBUG
	Q_EXPORT_PLUGIN2(Tuto5Geom, Tuto5Geom)
#else
	Q_EXPORT_PLUGIN2(Tuto5GeomD, Tuto5Geom)
#endif
