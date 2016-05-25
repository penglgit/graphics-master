#include "gl/glew.h"
#include "MeGlWindow.h"
#include <iostream>
#include <fstream>
using namespace std;

void SendDataToOpenGL()
{
	/*
	GLfloat verts[] = 
	{
		0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 1.0f,
		0.0f, 1.0f, 0.0f,

		-1.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

		-1.0f, -1.0f,
		0.0f, 1.0f, 0.0f,

		1.0f, -1.0f,
		0.0f, 0.0f, 1.0f,		
	};
	*/

	GLfloat verts[] = 
	{
		+0.5f, +0.5f, +0.5f,// v0
		+1.0f, +1.0f, +1.0f,// color

		-0.5f, +0.5f, +0.5f,// v1
		+1.0f, +0.0f, +1.0f,

		-0.5f, -0.5f, +0.5f,// v2
		+1.0f, +0.0f, +0.0f,

		+0.5f, -0.5f, +0.5f,// v3
		+1.0f, +1.0f, +0.0f,

		+0.5f, -0.5f, -0.5f,// v4
		+0.0f, +1.0f, +0.0f,

		+0.5f, +0.5f, -0.5f,// v5
		+0.0f, +1.0f, +1.0f,

		-0.5f, +0.5f, -0.5f,// v6
		+0.0f, +0.0f, +1.0f,

		-0.5f, -0.5f, -0.5f,// v7	
		+0.0f, +0.0f, +0.0f,
	};

	GLuint vertexBufferID;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), 
		verts, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (char *)(sizeof(float) * 3));

	// front, right, up, left, down, back
	GLushort indices[] = {
		0, 1, 2, 
		0, 2, 3,

		0, 3, 4,
		0, 4, 5,

		0, 5, 6,
		0, 6, 1,

		1, 6, 7,
		1, 7, 2,

		7, 4, 3,
		7, 3, 2,

		4, 7, 6,
		4, 6, 5,	
	};
	GLuint indexBufferID;
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),
		indices, GL_STATIC_DRAW);

}

// GLSL±‡“Î¡¨Ω”¥ÌŒÛºÏ≤È
bool CheckStatus( 
	GLuint objectID,
	PFNGLGETSHADERIVPROC obiectPropertyGetterFunc,
	PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
	GLenum statusType)
{
	GLint status;
	obiectPropertyGetterFunc(objectID, statusType, &status);
	if (status != GL_TRUE)
	{
		GLint infoLogLength;
		obiectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
		cout << buffer << endl;
		delete []buffer;
		return false;
	}
	return true;
}
bool CheckShaderStatus(GLuint shaderID)
{
	return CheckStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}
bool CheckProgramStatus(GLuint programID)
{
	return CheckStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

string ReadShaderCode(const char *fileName)
{
	ifstream meInput(fileName);
	if (!meInput.good())
	{
		cout << "File failed to load... " << fileName << endl;
		exit(1);
	}

	return std::string(
		std::istreambuf_iterator<char>(meInput),
		std::istreambuf_iterator<char>());
}

void InstallShaders()
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const char *adapter[1];
	string temp = ReadShaderCode("vertexShaderCode.vert");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	temp = ReadShaderCode("fragmentShaderCode.frag");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!CheckShaderStatus(vertexShaderID) || !CheckShaderStatus(fragmentShaderID))
		return;

	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	if (!CheckProgramStatus(programID))
		return;

	glUseProgram(programID);
}

void MeGlWindow::initializeGL()
{
	glewInit();
	glEnable(GL_DEPTH_TEST);

	SendDataToOpenGL();
	InstallShaders();
}

void MeGlWindow::paintGL()
{
	glViewport(0, 0, width(), height());
	// set clear color
	glClearColor(0, 0, 0, 1);

	// clear, clear color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	// draw point
	//glPointSize(50);	
	//glDrawArrays(GL_POINTS, 0, 1);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
}
