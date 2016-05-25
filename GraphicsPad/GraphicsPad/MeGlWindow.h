#ifndef ME_GL_WINDOW
#define ME_GL_WINDOW
#include <QtOpenGL/QGLWidget>


class MeGlWindow : public QGLWidget
{
public:
	void initializeGL();
	void paintGL();
};

#endif
