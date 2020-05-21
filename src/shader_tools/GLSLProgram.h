#pragma once
#include "shader_tools_common.h"
#include "GLSLShader.h"

class GLSLProgram {
public:
	GLuint programID;
	bool linked;
	std::string program_name; // Shader name
	GLSLShader* vertex_shader;
	GLSLShader* fragment_shader;
public:
	GLSLProgram::GLSLProgram();
	GLSLProgram::GLSLProgram(const std::string& program_name, GLSLShader* vertex, GLSLShader* fragment);
	void GLSLProgram::compile();
	void GLSLProgram::use();
	std::string GLSLProgram::getIDString();
private:
	void GLSLProgram::printLinkError(GLuint program);
};