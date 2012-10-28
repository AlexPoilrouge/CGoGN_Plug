#ifndef _TUTO5TOPO_H_
#define _TUTO5TOPO_H_


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

class Tuto5Topo : public VisualPlugin{
	Q_OBJECT
	Q_INTERFACES(Plugin)
public:
	Tuto5Topo();
	~Tuto5Topo(){}

	void cb_updateMatrix(View* view);

	void cb_redraw(Scene* scene);

	void cb_initGL(Scene* scene);

protected:
	MAP* myMap;
	MapHandler* m_vizuHandler;
	VertexAttribute<VEC3> position;

	Algo::Render::GL2::Topo3Render* m_render_topo;

	Utils::VBO* m_positionVBO;

	Dart m_selected;

	Utils::GL_Matrices m_mat;
	glm::mat4& m_transfo_matrix;



	bool activate();
	void disable();

};




#endif
