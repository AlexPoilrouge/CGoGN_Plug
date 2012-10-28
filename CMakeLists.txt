cmake_minimum_required(VERSION 2.8)

project(Visualizator)

SET( QT_USE_QTXML TRUE )

SET (QT_USE_QTDESIGNER TRUE)

SET (QT_USE_QTGUI TRUE)

SET(QT_USE_QTPLUGIN TRUE)

#========CGoGN========#
SET(CGoGN_ROOT_DIR ${CMAKE_SOURCE_DIR}/../../CGoGN CACHE STRING "CGoGN root dir")
INCLUDE(${CGoGN_ROOT_DIR}/apps_cmake.txt)


#=======QGLViewer=========#
SET(QGLVIEWER_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/../../QGLViewer)
SET(QGLVIEWER_INCLUDE_LIBRARY ${CMAKE_SOURCE_DIR}/../../QGLViewer/QGLViewer/libQGLViewer.so )



#======Directories=======#
SET(H_DIR ${CMAKE_SOURCE_DIR}/headers)
SET(SRC_DIR ${CMAKE_SOURCE_DIR}/source)
SET(FORMS_DIR ${CMAKE_SOURCE_DIR}/forms)
SET(RESOURCES_DIR ${CMAKE_SOURCE_DIR}/resources)

SET(PLUGIN_DIR ${CMAKE_SOURCE_DIR}/plugins)
SET(PLUGIN_COMPILE_DIR ${EXECUTABLE_OUTPUT_PATH}/plugins)


ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/Release Release)
IF (NOT WIN32)
	ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/Debug Debug)
ENDIF (NOT WIN32)

configure_file(${CMAKE_SOURCE_DIR}/Settings/state_save.xml ${EXECUTABLE_OUTPUT_PATH}/state_save.xml @ONLY)


find_package(Doxygen)
if(DOXYGEN_FOUND)
configure_file(${CMAKE_SOURCE_DIR}/documentation/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
add_custom_target(doc
${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Doc
COMMENT "Generating API documentation with Doxygen" VERBATIM
)
endif(DOXYGEN_FOUND)

