#pragma once
#include "shader_tools_common.h"

class GLSLProgram {
public:
	GLuint programID;
	bool linked;
	std::string program_name; // Shader name
	std::string vertex_path;
	std::string fragment_path;
public:
	GLSLProgram::GLSLProgram();
	GLSLProgram::GLSLProgram(const std::string& program_name, const std::string& vertex_path, const std::string& fragment_path);
	
	void GLSLProgram::compile();
	void GLSLProgram::use();
	std::string GLSLProgram::getIDString();
private:
	GLuint GLSLProgram::compileShader(const std::string& shader_path, GLenum shadertype);
	void GLSLProgram::printLinkError(GLuint program);
};