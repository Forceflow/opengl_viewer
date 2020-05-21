#pragma once
#include "shader_tools_common.h"

class GLSLShader {
public:
	GLuint shaderID; // OpenGL-assigned ID
	GLint compiled; // Whether or not this shader was compiled already
	GLenum shadertype; // GLEnum depicting shader type (VERTEX or FRAGMENT)
	std::string shader_name; // Shader name
private:
	std::string shader_src; // internal string representation of shader

public:
	GLSLShader::GLSLShader();
	GLSLShader::GLSLShader(const std::string& shader_name, GLenum shadertype);
	GLSLShader::GLSLShader(const std::string& shader_name, const char *shader_text, GLenum shadertype);
	GLSLShader::GLSLShader(const std::string& shader_name, const std::string& shader_text, GLenum shadertype);
	std::string GLSLShader::getSrc() const; 
	void GLSLShader::loadFromFile(const char* filepath);
	void GLSLShader::setSrc(const char* new_source);
	void GLSLShader::compile();
	std::string GLSLShader::getIDString();

private:
	void GLSLShader::getCompilationError(GLuint shader);
};