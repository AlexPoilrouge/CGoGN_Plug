#ifndef _CAMERAPATH_H_
#define _CAMERAPATH_H_


#include "visualPlugin.h"

class CameraPathDialog;


class CameraPath : public VisualPlugin{
	Q_OBJECT
	Q_INTERFACES(Plugin)
public:
	CameraPath();
	~CameraPath(){}

	bool activate();
	void disable();

	void cb_redraw(Scene* scene){}
	void cb_initGL(Scene* scene);


	virtual void cb_recievedScene(Scene* scene);
	virtual void cb_removingScene(Scene* scene);

	void closePathDialog();

protected:
	CameraPathDialog* m_cpd;

	ViewButton* m_movieButton;

public slots:
	void cameraDialog(View* view);
};

#endif
