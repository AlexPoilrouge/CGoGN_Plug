#include "tuto5Adds.h"


Tuto5Adds::Tuto5Adds() :
	myMap(NULL),
	m_vizuHandler(NULL),
	m_render(NULL),
	m_positionVBO(NULL),
	m_dataVBO(NULL),
	m_lines(NULL),
	m_strings(NULL),
	m_sprite(NULL),
	m_transfo_matrix(m_mat.m_matrices[2]),
	m_vboOK(false)
{
	m_transfo_matrix = glm::mat4(1.0f);
}


bool Tuto5Adds::activate(){
	waitingForScene(1);

//#ifndef DEBUG
//	Plugin* dep=this->getDependencie<Plugin>("t5_part1");
//#else
//	Plugin* dep= this->getDependencie<Plugin>("t5_part1_D");
//#endif
//	if(!dep){
//		System::Error::showError(window);
//		return false;
//	}

	m_vizuHandler= this->getReferencedMap("T5_Map");
	if(!m_vizuHandler){
		System::Error::showError();

		return false;
	}
	myMap= (MAP*)m_vizuHandler->map();

	position= myMap->getAttribute<PFP::VEC3, VERTEX>("position");

//	if(!this->associateGLViewer("t5Parts_glv",m_glviewer,true)){
//		Error::showError(window);
//		return false;
//	}


	GLint texSize;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texSize);


	return true;
}

void Tuto5Adds::disable(){
	this->suppressAllDependencieLinks();


	unregisterShader(m_strings);
	unregisterShader(m_lines);
	unregisterShader(m_sprite);

	if(m_lines)
		delete m_lines;
	if(m_dataVBO)
		delete m_dataVBO;
	if(m_strings)
		delete m_strings;
	if(m_sprite)
		delete m_sprite;
	if(m_positionVBO)
		delete m_positionVBO;
//	if(m_glviewer)
//		m_glviewer->suppressLinkWith(this);

//	std::cout << "This plugin (" << name.toStdString() <<  ") isn't active..." << std::endl;
}

void Tuto5Adds::cb_initGL(Scene* scene){
	if(scene){
//		RenderContainer* rc= m_glviewer->getRenderContainer();
		m_render = new Algo::Render::GL2::MapRender();

		// create VBO for position
//		m_positionVBO = new Utils::VBO();
//		m_positionVBO= rc->getVBO("positionVBO");
		VBOHandler* vbo;
		if((vbo=m_vizuHandler->findVBO("positionVBO"))){
			m_positionVBO= vbo;
		}
		else{
			m_positionVBO = new VBOHandler("positionVBO");
			m_vizuHandler->addVBO(m_positionVBO);
		}
		m_positionVBO->updateData(position);

		if(!m_sprite){
			m_sprite = new Utils::PointSprite();
	//		rc->addAndRegisterShader("SpriteShader",m_sprite);
			m_sprite->setAttributePosition(m_positionVBO);
		}

		if(!m_strings){
		m_strings = new Utils::Strings3D(true, Geom::Vec3f(0.1f,0.0f,0.3f));
//			rc->addAndRegisterShader("StringsShader",m_strings);
		storeVerticesInfo();
		m_strings->sendToVBO();
		}

		// copy de contenu de VBO a la creation
//			rc->addVBO("dataVBO",m_dataVBO);
		if((vbo=m_vizuHandler->findVBO("dataVBO"))){
			m_dataVBO= vbo;
		}
		else{
			m_dataVBO = new VBOHandler(*m_positionVBO, "dataVBO");
			m_vizuHandler->addVBO(m_dataVBO);
		}

		if(!m_lines){
		m_lines = new Utils::ShaderVectorPerVertex();
//			rc->addAndRegisterShader("linesShader",m_lines);
		m_lines->setAttributePosition(m_positionVBO);
		m_lines->setAttributeVector(m_dataVBO);
		m_lines->setScale(0.2f);
		m_lines->setColor(Geom::Vec4f(0.0f, 1.0f, 0.2f, 0.0f));
		}

		// accede au buffer du VBO pour modification
//		PFP::VEC3* data = static_cast<PFP::VEC3*>(m_dataVBO->lockPtr());
//		for (unsigned int i=0; i< m_dataVBO->nbElts(); ++i)
//		{
//			data[i].normalize();
//		}
//		m_dataVBO->releasePtr();

		std:: cout << "coocoo" << std::endl;

		registerShader(m_strings);
		registerShader(m_sprite);
		registerShader(m_lines);

//		SelectorTrue allDarts;

		m_render->initPrimitives<PFP>(*myMap, allDarts, Algo::Render::GL2::TRIANGLES);
		m_render->initPrimitives<PFP>(*myMap, allDarts, Algo::Render::GL2::LINES);
		m_render->initPrimitives<PFP>(*myMap, allDarts, Algo::Render::GL2::POINTS);



		m_sprite->setSize(0.025f);
		m_lines->setScale(0.1f);
		m_strings->setScale(0.01f);

	}
}

void Tuto5Adds::cb_redraw(Scene* scene){
	if(scene){
		std::cout << "redraw t5part3" << std::endl;

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_LIGHTING);
		glPolygonOffset(1.0f, 1.0f);
		m_strings->drawAll(Geom::Vec3f(0.0f, 1.0f, 1.0f));

		m_sprite->predraw(Geom::Vec3f(1.0f, 0.0f ,0.0f));
		m_render->draw(m_sprite, Algo::Render::GL2::POINTS);
		m_sprite->postdraw();

		glLineWidth(1.0f);
		m_render->draw(m_lines, Algo::Render::GL2::POINTS);
		std::cout << "end redraw" <<std::endl;
	}
}


void Tuto5Adds::cb_updateMatrix(View* view)
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

void Tuto5Adds::registerShader(Utils::GLSLShader* ptr)
{
	Utils::GLSLShader::registerShader(NULL, ptr) ;
}

void Tuto5Adds::unregisterShader(Utils::GLSLShader* ptr)
{
	Utils::GLSLShader::unregisterShader(NULL, ptr) ;
}

void Tuto5Adds::storeVerticesInfo()
{
	CellMarker<VERTEX> mv(*myMap);
	for (Dart d = myMap->begin(); d != myMap->end(); myMap->next(d))
	{
		if (!mv.isMarked(d))
		{
			mv.mark(d);
			std::stringstream ss;
			ss << d << " : "<< position[d];
			m_strings->addString(ss.str(), position[d]);
		}
	}
}



#ifndef DEBUG
	Q_EXPORT_PLUGIN2(Tuto5Adds, Tuto5Adds)
#else
	Q_EXPORT_PLUGIN2(Tuto5AddsD, Tuto5Adds)
#endif
