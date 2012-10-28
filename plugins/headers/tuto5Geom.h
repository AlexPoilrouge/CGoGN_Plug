#ifndef _TUTO5GEOM_H_
#define _TUTO5GEOM_H_

#include "visualPlugin.h"

#include "Utils/GLSLShader.h"

#include "Utils/gl_matrices.h"
#include "Utils/cgognStream.h"
#include "Geometry/vector_gen.h"


#include "Topology/generic/parameters.h"

#ifdef USE_GMAP
	#include "Topology/gmap/embeddedGMap3.h"
#else
	#include "Topology/map/embeddedMap3.h"
#endif

#include "Geometry/vector_gen.h"
#include "Algo/Geometry/boundingbox.h"
#include "Algo/Render/GL2/mapRender.h"


#include "Algo/Render/GL2/topo3Render.h"

#include "Topology/generic/cellmarker.h"
#include "Utils/text3d.h"

#include "Utils/pointSprite.h"
#include "Utils/Shaders/shaderSimpleColor.h"
#include "Utils/Shaders/shaderVectorPerVertex.h"
#include "Utils/cgognStream.h"


using namespace CGoGN ;

struct PFP: public PFP_STANDARD
{
	// definition de la carte
#ifdef WITH_GMAP
	typedef EmbeddedGMap3 MAP;
#else
	typedef EmbeddedMap3 MAP;
#endif
};

typedef PFP::MAP MAP ;
typedef PFP::VEC3 VEC3 ;

class Tuto5Geom : public VisualPlugin{
	Q_OBJECT
	Q_INTERFACES(Plugin)
public:
	Tuto5Geom();
	~Tuto5Geom();

	void transfoRotate(float angle, float x, float y, float z);

	void cb_updateMatrix(View* view);

	void cb_redraw(Scene* scene);

	void cb_initGL(Scene* scene);

	bool cb_keyPress(Scene* scene, int event);

	void cb_recievedMap(MapHandler* map);
	void cb_removingMap(MapHandler* map);

	bool activate();
	void disable();


protected:

	MAP* myMap;
	MapHandler* m_vizuHandler;
	VertexAttribute<VEC3> position ;

	Utils::GL_Matrices m_mat;
	Scene* m_scene;


	glm::mat4& m_transfo_matrix;

	QTimer *m_timer;

	Dart m_selected;

	Algo::Render::GL2::MapRender* m_render;

	Utils::ShaderSimpleColor* m_shader;

//	Utils::VBO* m_positionVBO;
	VBOHandler* m_positionVBO;

	bool m_init;

public slots:

	void animate();
};


#endif
