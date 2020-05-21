#include "GLSLProgram.h"

GLSLProgram::GLSLProgram() : programID(0), vertex_shader(NULL), fragment_shader(NULL), linked(false) {}

GLSLProgram::GLSLProgram(const std::string& program_name, GLSLShader* vertex, GLSLShader* fragment) : 
	programID(0), program_name(program_name), vertex_shader(vertex), fragment_shader(fragment), linked(false) {}

void GLSLProgram::compile() {
	// create empty program
	programID = glCreateProgram();
	// try to attach all shaders
	GLSLShader* shaders[2] = { vertex_shader, fragment_shader };
	for (unsigned int i = 0; i < 2; i++) {
		if (shaders[i] != NULL) {
			if (!shaders[i]->compiled) { shaders[i]->compile(); } // try to compile shader if not yet compiled
			if (shaders[i]->compiled) {
				glAttachShader(programID, shaders[i]->shaderID);
				printf("(P) (%s) Attached shader %s\n", getIDString().c_str(), shaders[i]->getIDString().c_str());
			}
			else {
				printf("(P) (%s) Failed to attach shader %s", getIDString().c_str(), shaders[i]->getIDString().c_str());
				glDeleteProgram(programID);
				return;
			}
		}
	}
	// try to link program
	glLinkProgram(programID);
	GLint isLinked = 0;
	glGetProgramiv(programID, GL_LINK_STATUS, &isLinked); // check if program linked
	if (isLinked == GL_FALSE) {
		printLinkError(programID);
		glDeleteProgram(programID);
		linked = false;
	}
	else {
		linked = true;
		printf("(P) (%s) Linked program \n", getIDString().c_str());
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

std::string GLSLProgram::getIDString() {
	if (programID == 0)
		return program_name;
	else
		return program_name + ":" + std::to_string(programID);
}