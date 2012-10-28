#include "cameraPathDialog.h"


#include "cameraPath.h"
#include "QGLViewer/frame.h"

#include <QMessageBox>
#include <QDomDocument>
#include <QFileInfo>
#include <QFileDialog>
#include <QTextStream>
#include <QMap>

Q_DECLARE_METATYPE(qglviewer::Frame);

CameraPathDialog::CameraPathDialog(Camera* camera, CameraPath* creator, QWidget* parent) : QMainWindow(parent),
	m_camera(camera),
	m_creator(creator),
	b_playing(false)
{
	this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);

	System::Events::movieDialogOpened= true;

	connect(m_camera, SIGNAL(destroyed()), this, SLOT(close()));

	this->setupUi(this);

	m_kfi= m_camera->keyFrameInterpolator(0);
	if(!m_kfi){
		m_kfi= new qglviewer::KeyFrameInterpolator(m_camera->frame());
		m_camera->setKeyFrameInterpolator(0,m_kfi);
	}


	camera_line->setText(m_camera->getName());

	m_speed= speed_spin->value();

	b_gotoFrame= goToFrame_check->isChecked();

	b_savePlay= saveReplay_check->isChecked();

	m_snapName= snapName_line->text();
	if(m_snapName.isEmpty()){
		m_snapName= QString("snapshot");
		snapName_line->setText(m_snapName);
	}

	m_snapDirPath= dirSnap_line->text();
	if(m_snapDirPath.isEmpty() || !QFileInfo(m_snapDirPath).isDir()){
		m_snapDirPath= System::app_path + "/snapshots";
		dirSnap_line->setText(m_snapDirPath);
	}
	m_snapDir= QDir(m_snapDirPath);

	changeSnapFrameRate(frame_spin->value());

	connect(play_button, SIGNAL(pressed()), this, SLOT(play()));
	connect(restart_button, SIGNAL(pressed()), this, SLOT(restart()));
	connect(m_kfi, SIGNAL(endReached()), this, SLOT(stop()));
	connect(speed_spin, SIGNAL(valueChanged(int)), this, SLOT(changeSpeed(int)));
	connect(addFrame_button, SIGNAL(pressed()), this, SLOT(addFrame()));
	connect(removeFrame_button, SIGNAL(pressed()), this, SLOT(removeFrame()));
	connect(goToFrame_check, SIGNAL(toggled(bool)), this, SLOT(setGotoSelectedFrame(bool)));
	connect(erasePath_check, SIGNAL(pressed()), this, SLOT(deletePath()));
	connect(saveReplay_check, SIGNAL(toggled(bool)), this, SLOT(setSaveSnapshot(bool)));
	connect(snapName_line, SIGNAL(textEdited(const QString)), this, SLOT(textSnapshotName(const QString)));
	connect(dirSnap_line, SIGNAL(textChanged(QString)), this, SLOT(textSnapshotDir(QString)));
	connect(browse_button, SIGNAL(pressed()), this, SLOT(browseDir()));
	connect(frame_spin, SIGNAL(valueChanged(int)),this, SLOT(changeSnapFrameRate(int)));
	connect(load_button, SIGNAL(pressed()), this, SLOT(loadPath()));
	connect(save_button, SIGNAL(pressed()), this, SLOT(savePath()));

	refreshList();

	connect(listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(gotoCurrentFrame(QListWidgetItem*)));
	connect(listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(changeFrameTime(QListWidgetItem*)));
	connect(m_kfi, SIGNAL(interpolated()), this, SLOT(interpolation()));
}

CameraPathDialog::~CameraPathDialog(){}


void CameraPathDialog::cameraGotoFrame(qglviewer::Frame frame){
	m_camera->setPosition(frame.position());
	m_camera->setOrientation(frame.orientation());
	m_camera->updateGL();
}

void CameraPathDialog::refreshList(){
	listWidget->clear();

	if(m_kfi){
		int t= m_kfi->numberOfKeyFrames();
		for(int i=0; i<t; ++i){
			QListWidgetItem* item= new QListWidgetItem(QString("KeyFrame")+QString::number(i)+"\t("+QString::number(m_kfi->keyFrameTime(i))+"s.)"
														, listWidget);
			QVariant var;
			var.setValue(m_kfi->keyFrame(i));
			item->setData(Qt::UserRole, var);
		}
	}
}

void CameraPathDialog::play(){
	if(m_kfi->numberOfKeyFrames()>0){
		b_playing= !b_playing;
		if(b_playing){
			m_snapDir= QDir(m_snapDirPath);
			if(!m_snapDir.exists()){
				if(m_snapDir.mkpath(m_snapDir.absolutePath())){
					m_snapDirPath= m_snapDir.absolutePath();
				}
				else{
					m_snapDirPath=".";
				}
			}
			play_button->setText("Pause");

			if(b_savePlay){
				m_camera->viewShowButton(false);
			}
		}
		else{
			play_button->setText("Play");

			if(b_savePlay){
				m_camera->viewShowButton(true);
			}
		}
		m_camera->playPath(0);
	}
}

void CameraPathDialog::restart(){
	if(b_playing){
		m_camera->playPath(0);
		b_playing= false;
	}
	m_camera->viewShowButton(true);
	m_camera->resetPath(0);
	m_camera->resetSnapCount();
}

void CameraPathDialog::stop(){
	m_camera->resetPath(0);
	m_camera->resetSnapCount();
	play_button->setText("Play");
	m_camera->viewShowButton(true);
	b_playing= false;
}

void CameraPathDialog::changeSpeed(int speed){
	m_speed= speed;
	m_kfi->setInterpolationSpeed(((float)speed)/100.0);
}

void CameraPathDialog::addFrame(){
	m_kfi->addKeyFrame(new qglviewer::Frame(*m_camera->frame()));
	std::cout << "adding kf " << m_camera->frame() << std::endl;

	refreshList();
	m_camera->updateGL();
}

void CameraPathDialog::removeFrame(){
	QListWidgetItem* item= listWidget->currentItem();
	if(item){
		int t= m_kfi->numberOfKeyFrames();
		int i= listWidget->currentRow();

		QMap<float, qglviewer::Frame> frameMap;
		for(int j=0; j<t; ++j){
			if(i!=j){
				frameMap.insert(m_kfi->keyFrameTime(j), m_kfi->keyFrame(j));
			}
		}

		m_camera->resetPath(0);

		qglviewer::KeyFrameInterpolator* new_kfi= new qglviewer::KeyFrameInterpolator(m_camera->frame());

		for(QMap<float, qglviewer::Frame>::iterator it= frameMap.begin(); it!=frameMap.end(); ++it){
			new_kfi->addKeyFrame(it.value(),it.key());
		}

		disconnect(m_kfi, SIGNAL(endReached()), this, SLOT(stop()));
		disconnect(m_kfi, SIGNAL(interpolated()), this, SLOT(interpolation()));
		delete m_kfi;
		m_kfi= new_kfi;
		m_camera->setKeyFrameInterpolator(0,m_kfi);
		connect(m_kfi, SIGNAL(endReached()), this, SLOT(stop()));
		connect(m_kfi, SIGNAL(interpolated()), this, SLOT(interpolation()));

		refreshList();
		m_camera->updateGL();
	}
}

void CameraPathDialog::setGotoSelectedFrame(bool b){
	b_gotoFrame= b;
	if(b && listWidget->currentItem()){
		cameraGotoFrame(listWidget->currentItem()->data(Qt::UserRole).value<qglviewer::Frame>());
	}
}

void CameraPathDialog::gotoCurrentFrame(QListWidgetItem* item){
	if(b_gotoFrame){
		cameraGotoFrame(item->data(Qt::UserRole).value<qglviewer::Frame>());
	}
}

void CameraPathDialog::deletePath(){
	m_kfi->deletePath();

	refreshList();
	m_camera->updateGL();
}

void CameraPathDialog::setSaveSnapshot(bool b){
	b_savePlay= b;
}

void CameraPathDialog::textSnapshotName(const QString & text){
	if(m_snapName.isEmpty()){
		m_snapName= QString("snapshot");
		snapName_line->setText(m_snapName);
	}
	else{
		m_snapName= text;
	}
}

void CameraPathDialog::textSnapshotDir(QString text){
	m_snapDirPath= text;
}

void CameraPathDialog::browseDir(){
	QString text=QFileDialog::getExistingDirectory(this,"Snapshot directory");
	dirSnap_line->setText(text);
}

void CameraPathDialog::changeSnapFrameRate(int rate){
	if(rate>=0){
		m_frameRate= rate;
		m_snapPeriod= 1.0/((float)rate);
		m_kfi->setInterpolationPeriod((int)(m_snapPeriod*1000));
	}
}

void CameraPathDialog::loadPath(){
	m_pathSaveFile= QFileDialog::getOpenFileName(this, "Open XML path file", ".", "Path files (*.path.xml)");

	QFile file(m_pathSaveFile);
	if(!file.open(QIODevice::ReadOnly)){
		file.close();
		System::Error::code= System::Error::OPEN_FILE_ERROR_f(m_pathSaveFile);
		System::Error::showError(this);

		return;
	}

	QDomDocument doc;
	if(!doc.setContent(&file)){
		file.close();
		System::Error::code= System::Error::BAD_XML_FILE_f(m_pathSaveFile);
		System::Error::showError(this);

		return;
	}

	QDomElement mainElem= doc.documentElement();
	QDomElement pathElem;
	if((pathElem=mainElem.firstChildElement()).isNull()){
		file.close();
		System::Error::code= System::Error::BAD_XML_FILE_f(m_pathSaveFile);
		System::Error::showError(this);

		return;
	}

	m_kfi->initFromDOMElement(pathElem);

	file.close();

	refreshList();
	m_camera->updateGL();

	speed_spin->setValue((int)(m_kfi->interpolationSpeed()*100));

	m_snapPeriod= ((float)(m_kfi->interpolationPeriod()))/1000;
	m_frameRate= ((int)(1.0/m_snapPeriod));
	frame_spin->setValue(m_frameRate);
}

void CameraPathDialog::savePath(){
	QFileInfo info(m_pathSaveFile);

	QString outFilePath;
	if(m_pathSaveFile.isEmpty() || !info.exists()){
		outFilePath= QFileDialog::getSaveFileName(this, "Sauver le chemin", ".", "Path file (*.path.xml)");
	}
	else{
		outFilePath= QFileDialog::getSaveFileName(this, "Sauver le chemin", m_pathSaveFile, "Path file (*.path.xml)");
	}

	QFile outFile(outFilePath);
	if(!outFile.open(QIODevice::WriteOnly)){
		System::Error::code= System::Error::OPEN_FILE_ERROR_f(outFilePath);
		System::Error::showError(this);
		outFile.close();

		return;
	}

	QDomDocument doc("pathDocument");
	QDomElement rootElem= doc.createElement("CAMERA_PATH");
	doc.appendChild(rootElem);

	rootElem.appendChild(m_kfi->domElement(m_camera->getName()+"_path_0", doc));


	QTextStream out;
	out.setDevice(&outFile);
	doc.save(out,2);

	outFile.close();
}

void CameraPathDialog::interpolation(){
	m_camera->updateGL();
	if(b_playing && b_savePlay){
		m_camera->saveSnapshot(m_snapDirPath+'/'+m_snapName);
	}
}

void CameraPathDialog::closeEvent(QCloseEvent* event){
	m_creator->closePathDialog();
}

void CameraPathDialog::changeFrameTime(QListWidgetItem* item){
	int i= listWidget->row(item);
	int t;
	if(i>=0 && i<(t=m_kfi->numberOfKeyFrames())){
		double min, max;
		if(i==0){
			min=0.0;
			if(t>1){
				max= m_kfi->keyFrameTime(i+1);
			}
			else{
				max= 3599.99;
			}
		}
		else if(i==t-1){
			min= m_kfi->keyFrameTime(i-1);
			max= 3599.99;
		}
		else{
			min= m_kfi->keyFrameTime(i-1);
			max= m_kfi->keyFrameTime(i+1);
		}

		FrameTimeDialog ftd(m_kfi->keyFrameTime(i), 0.0, 3599.99, this);

		if(ftd.exec()==QDialog::Accepted){
			std::cout << "accepted" << std::endl;
			QMap<float, qglviewer::Frame> frameMap;
			for(int j=0; j<t; ++j){
				if(i!=j){
					frameMap.insert(m_kfi->keyFrameTime(j), m_kfi->keyFrame(j));
				}
				else{
					frameMap.insert((float)ftd.time(), m_kfi->keyFrame(i));
				}
			}

			m_camera->resetPath(0);

			qglviewer::KeyFrameInterpolator* new_kfi= new qglviewer::KeyFrameInterpolator(m_camera->frame());

			for(QMap<float, qglviewer::Frame>::iterator it= frameMap.begin(); it!=frameMap.end(); ++it){
				new_kfi->addKeyFrame(it.value(),it.key());
			}

			disconnect(m_kfi, SIGNAL(endReached()), this, SLOT(stop()));
			disconnect(m_kfi, SIGNAL(interpolated()), this, SLOT(interpolation()));
			delete m_kfi;
			m_kfi= new_kfi;
			m_camera->setKeyFrameInterpolator(0,m_kfi);
			connect(m_kfi, SIGNAL(endReached()), this, SLOT(stop()));
			connect(m_kfi, SIGNAL(interpolated()), this, SLOT(interpolation()));


			m_camera->updateGL();
			refreshList();
		}
	}
}






FrameTimeDialog::FrameTimeDialog(double time, double timeMin, double timeMax, QWidget* parent) : QDialog(parent)
{
	this->setupUi(this);

	doubleSpinBox->setValue(time);

	doubleSpinBox->setMinimum(timeMin);
	doubleSpinBox->setMaximum(timeMax);

	m_time= doubleSpinBox->value();

	connect(doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(timeChange(double)));
}

void FrameTimeDialog::timeChange(double t){
	m_time= t;
}


