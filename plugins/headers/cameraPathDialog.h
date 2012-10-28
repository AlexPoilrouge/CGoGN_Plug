#ifndef _CAMERAPATHDIALOG_H_
#define _CAMERAPATHDIALOG_H_


#include "ui_cameraPathDialog.h"

#include "ui_frameTimeDialog.h"

#include <QDir>

class Camera;
class CameraPath;
namespace qglviewer{
	class KeyFrameInterpolator;
	class Frame;
}

class CameraPathDialog : public QMainWindow, Ui::CPDialog{
	Q_OBJECT
public:
	CameraPathDialog(Camera* camera, CameraPath* creator, QWidget* parent=0);
	~CameraPathDialog();


	void closeEvent(QCloseEvent* event);

protected:
	Camera* m_camera;
	qglviewer::KeyFrameInterpolator* m_kfi;

	CameraPath* m_creator;

	int m_speed;

	bool b_gotoFrame;

	bool b_savePlay;

	QString m_snapName;

	QString m_snapDirPath;
	QDir m_snapDir;

	int m_frameRate;

	QString m_pathSaveFile;

	float m_snapPeriod;

	bool b_playing;


	void cameraGotoFrame(qglviewer::Frame frame);

	void refreshList();


public slots:
	void play();
	void restart();
	void stop();
	void changeSpeed(int speed);
	void addFrame();
	void removeFrame();
	void setGotoSelectedFrame(bool b);
	void gotoCurrentFrame(QListWidgetItem* item);
	void deletePath();
	void textSnapshotName(const QString &text);
	void setSaveSnapshot(bool b);
	void textSnapshotDir(QString text);
	void browseDir();
	void changeSnapFrameRate(int rate);
	void loadPath();
	void savePath();

	void interpolation();

	void changeFrameTime(QListWidgetItem* item);

};


class FrameTimeDialog : public QDialog, public Ui::FrameTimeDialog{
	Q_OBJECT
public:
	FrameTimeDialog(double time, double timeMin=0.0, double timeMax=3599.99, QWidget* parent=0);

	double time(){return m_time;}


protected:
	double m_time;

public slots:
	void timeChange(double t);
};


#endif
