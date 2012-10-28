#include "cameraPath.h"

#include "cameraPathDialog.h"

CameraPath::CameraPath() :
	m_cpd(NULL),
	m_movieButton(NULL)
{}

bool CameraPath::activate(){
	return true;
}

void CameraPath::disable(){
	disconnect(m_movieButton, SIGNAL(viewClicked(View*)), this, SLOT(cameraDialog(View*)));
	delete m_movieButton;
}

void CameraPath::cb_initGL(Scene* scene){
	if(!m_movieButton){
		m_movieButton= this->addSceneNewViewButton(scene, ":icon/movie.png");
		connect(m_movieButton, SIGNAL(viewClicked(View*)), this, SLOT(cameraDialog(View*)));
	}
}

void CameraPath::cb_recievedScene(Scene* scene){
	if(scene && m_movieButton){
		this->addSceneViewButton(scene, m_movieButton);
	}
}

void CameraPath::cb_removingScene(Scene* scene){
	if(scene){
		this->takeSceneViewButton(scene, m_movieButton);
	}
}

void CameraPath::cameraDialog(View* view){
	if(!m_cpd){
		m_cpd= new CameraPathDialog(view->currentCamera(), this, view);
		m_cpd->show();
	}
}

void CameraPath::closePathDialog(){
	if(m_cpd){
		delete(m_cpd);
		m_cpd=NULL;
	}
}


#ifndef DEBUG
	Q_EXPORT_PLUGIN2(CameraPath, CameraPath)
#else
	Q_EXPORT_PLUGIN2(CameraPathD, CameraPath)
#endif
