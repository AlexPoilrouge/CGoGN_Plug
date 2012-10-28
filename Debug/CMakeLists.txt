cmake_minimum_required(VERSION 2.8)

SET(CMAKE_BUILD_TYPE Debug)


IF(WIN32)
	link_directories( ${CGoGN_ROOT_DIR}/lib/Release )
ELSE (WIN32)
	link_directories( ${CGoGN_ROOT_DIR}/lib/Debug ${CGoGN_ROOT_DIR}/lib/Release )
ENDIF (WIN32)

INCLUDE_DIRECTORIES(
	${CGoGN_ROOT_DIR}/include
	${COMMON_INCLUDES}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${QGLVIEWER_INCLUDE_DIR}
)

SET(SRC_FILES
	${SRC_DIR}/main.cpp
	${SRC_DIR}/interface/window.cpp
	${SRC_DIR}/interface/system.cpp
	${SRC_DIR}/interface/stateHandler.cpp
	${SRC_DIR}/visualization/view.cpp
	${SRC_DIR}/visualization/scene.cpp
	${SRC_DIR}/visualization/camera.cpp
	${SRC_DIR}/interface/splitArea.cpp
	${SRC_DIR}/dialogs/newSceneDialog.cpp
	${SRC_DIR}/dialogs/sceneSelector.cpp
	${SRC_DIR}/interface/viewButtonArea.cpp
	${SRC_DIR}/dialogs/cameraViewDialog.cpp
	${SRC_DIR}/dialogs/cameraOptionDialog.cpp
	${SRC_DIR}/dialogs/cameraSceneDialog.cpp
	${SRC_DIR}/dialogs/globalCameraDialog.cpp
	${SRC_DIR}/dialogs/pluginDialog.cpp
	${SRC_DIR}/dialogs/linkViewDialog.cpp
	${SRC_DIR}/visualization/vboHandler.cpp
	${SRC_DIR}/visualization/mapHandler.cpp
	${SRC_DIR}/visualization/context.cpp
	${SRC_DIR}/dialogs/mapPluginDialog.cpp
)

SET(H_OBJECT_FILES
	${H_DIR}/interface/window.h
	${H_DIR}/interface/system.h
	${H_DIR}/visualization/view.h
	${H_DIR}/visualization/scene.h
	${H_DIR}/interface/splitArea.h
	${H_DIR}/dialogs/newSceneDialog.h
	${H_DIR}/dialogs/sceneSelector.h
	${H_DIR}/interface/viewButtonArea.h
	${H_DIR}/dialogs/cameraViewDialog.h
	${H_DIR}/dialogs/cameraOptionDialog.h
	${H_DIR}/dialogs/cameraSceneDialog.h
	${H_DIR}/dialogs/globalCameraDialog.h
	${H_DIR}/dialogs/pluginDialog.h
	${H_DIR}/dialogs/linkViewDialog.h
	${H_DIR}/dialogs/mapPluginDialog.h
)

SET(FORMS_FILES
	${FORMS_DIR}/window.ui
	${FORMS_DIR}/questionSessionDialog.ui
	${FORMS_DIR}/infoDialog.ui
	${FORMS_DIR}/newSceneDialog.ui
	${FORMS_DIR}/cameraViewDialog.ui
	${FORMS_DIR}/cameraOptionDialog.ui
	${FORMS_DIR}/cameraSceneDialog.ui
	${FORMS_DIR}/globalCameraDialog.ui
	${FORMS_DIR}/pluginDialog.ui
	${FORMS_DIR}/linkViewDialog.ui
	${FORMS_DIR}/mapPluginDialog.ui
)

QT4_WRAP_UI(UI_H_FILES
	${FORMS_FILES}
)
QT4_WRAP_CPP(MOC_FILES
	${H_OBJECT_FILES}
)
QT4_ADD_RESOURCES(RCC_FILES
	${RESOURCES_DIR}/resources.qrc
)

INCLUDE_DIRECTORIES(
	${FORMS}
	${H_DIR}
	${H_DIR}/interface
	${H_DIR}/visualization
	${H_DIR}/dialogs
	${H_DIR}/plugins
)

ADD_EXECUTABLE( CGoGN_plugD
	${SRC_FILES}
	${UI_H_FILES}
	${MOC_FILES}
	${RCC_FILES}
)

TARGET_LINK_LIBRARIES(CGoGN_plugD
	${CGoGN_LIBS_D}
	${COMMON_LIBS}
	${QGLVIEWER_INCLUDE_LIBRARY}
)

ADD_SUBDIRECTORY(${PLUGIN_DIR} plugins)

