=========================
########-INSTALL-########
=========================

In order to compile this project you must first build
this directory tree:

                  +----- CGoGN
                  |
----CGOGN_Apps----+----- QGLViewer
                  |
                  +----- SOCIS2012-----CGoGN_Plug
                  
Where:
	-CGoGN contains the CGoGN compiled library.
	-QGLViewer the compiled libQGLViewer library
	-CGoGN_Plug is this project's directory
	
Once done, create an empty build directory in this project's directory:
	
	# mkdir build
	# cd build
	# cmake ..
	# make
	
To download and compile CGoGN and QGLViewer, check their respective
web pages:
	 http://newlsiit.u-strasbg.fr/igg_fr/index.php/CiGoGN
	 http://www.libqglviewer.com/
	 
