#ifndef _TUTO5ADDS_H_
#define _TUTO5ADDS_H_

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

class Tuto5Adds : public VisualPlugin{
	Q_OBJECT
	Q_INTERFACES(Plugin)
public:
	Tuto5Adds();
	~Tuto5Adds(){}

	void cb_updateMatrix(View* view);

	void cb_redraw(Scene* scene);

	void cb_initGL(Scene* scene);

	void registerShader(Utils::GLSLShader* ptr);
	void unregisterShader(Utils::GLSLShader* ptr);

	void storeVerticesInfo();

protected:
	MAP* myMap;
	MapHandler* m_vizuHandler;
	VertexAttribute<VEC3> position;

	Scene* m_scene;

	Algo::Render::GL2::MapRender* m_render;

	VBOHandler* m_positionVBO;
	VBOHandler* m_dataVBO;

	Utils::ShaderVectorPerVertex* m_lines;
	Utils::Strings3D* m_strings;
	Utils::PointSprite* m_sprite;

	Utils::GL_Matrices m_mat;
	glm::mat4& m_transfo_matrix;

	bool m_vboOK;



	bool activate();
	void disable();

};




#endif
