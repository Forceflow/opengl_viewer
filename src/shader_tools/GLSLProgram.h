#pragma once
#include "shader_tools_common.h"
#include "GLSLShader.h"

class GLSLProgram {
public:
	GLuint program;
	bool linked;
private:
	GLSLShader* vertex_shader;
	GLSLShader* fragment_shader;
public:
	GLSLProgram();
	GLSLProgram(GLSLShader* vertex, GLSLShader* fragment);
	void compile();
	void use();
private:
	void printLinkError(GLuint program);
};