#include "GLSLProgram.h"

GLSLProgram::GLSLProgram() : programID(0), vertex_path(""), fragment_path(""), linked(false) {}

GLSLProgram::GLSLProgram(const std::string& program_name, const std::string& vertex_path, const std::string& fragment_path) :
	programID(0), program_name(program_name), vertex_path(vertex_path), fragment_path(fragment_path), linked(false) {}

void GLSLProgram::compile() {
	// delete previous program if there was any
	if (programID != 0) {
		glDeleteProgram(programID);
	}
	// create new program
	programID = glCreateProgram();
	// compile all shaders
	printf("(P) (%s) Compiling vertex shader %s ... ", getIDString().c_str(), vertex_path.c_str());
	GLuint vertexID = this->compileShader(vertex_path.c_str(), GL_VERTEX_SHADER);
	printf("(P) (%s) Compiling fragment shader %s ... ", getIDString().c_str(), fragment_path.c_str());
	GLuint fragmentID = this->compileShader(fragment_path.c_str(), GL_FRAGMENT_SHADER);
	// attach shaders
	glAttachShader(programID, vertexID);
	glAttachShader(programID, fragmentID);
	// link program
	printf("(P) (%s) Linking program ... ", getIDString().c_str());
	glLinkProgram(programID);
	GLint isLinked = 0;
	glGetProgramiv(programID, GL_LINK_STATUS, &isLinked); // check if program linked
	if (isLinked == GL_FALSE) {
		printf("Fail\n");
		printLinkError(programID);
		glDeleteProgram(programID);
		linked = false;
	}
	else {
		linked = true;
		printf("OK\n");
		// cleanup shader objects
		glDeleteShader(vertexID);
		glDeleteShader(fragmentID);
	}
}

void GLSLProgram::use() {
	glUseProgram(this->programID);
}

void GLSLProgram::printLinkError(GLuint program) {
	GLint infologLength = 0;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, (GLint *)&infologLength);
	char* infoLog = (char *)malloc(infologLength);
	glGetProgramInfoLog(program, infologLength, NULL, infoLog); // will include terminate char
	printf("(P) (%s) Program compilation error: %s\n", getIDString().c_str(), infoLog);
	free(infoLog);
}

GLuint GLSLProgram::compileShader(const std::string& shader_path, GLenum shadertype) {
	// Load from file
	std::string shader_src = loadFileToString(shader_path.c_str());
	// Compile
	GLuint id;GLint compiled;
	id = glCreateShader(shadertype);
	glShaderSource(id, 1, ShaderStringHelper(shader_src), NULL);
	glCompileShader(id);
	// check if shader compiled
	glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		int infologLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, (GLint*)&infologLength);
		char* infoLog = (char*)malloc(infologLength);
		glGetShaderInfoLog(id, infologLength, NULL, infoLog); // will include terminate char
		printf("Fail\n");
		printf("Shader %s compilation error:\n%s\n", shader_path.c_str(), infoLog);
		free(infoLog);
		glDeleteShader(id);
		compiled = false;
		return NULL;
	}
	else {
		printf("OK\n");
		compiled = true;
		return id;
	}
}

std::string GLSLProgram::getIDString() {
	if (programID == 0)
		return program_name;
	else
		return program_name + ":" + std::to_string(programID);
}