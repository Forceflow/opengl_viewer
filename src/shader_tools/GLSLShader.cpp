#include "GLSLShader.h"

GLSLShader::GLSLShader() :
	shaderID(0), compiled(false), shadertype(0), shader_name(""), shader_src("") {
}

GLSLShader::GLSLShader(const std::string& shader_name, GLenum shadertype) :
	shaderID(0), compiled(false), shadertype(shadertype), shader_name(shader_name), shader_src("") {
}

GLSLShader::GLSLShader(const std::string &shader_name, const char *shader_text, GLenum shadertype) :
	shaderID(0), compiled(false), shadertype(shadertype), shader_name(shader_name), shader_src(std::string(shader_text)) {
}

GLSLShader::GLSLShader(const std::string &shader_name, const std::string &shader_text, GLenum shadertype) :
	shaderID(0), compiled(false), shadertype(shadertype), shader_name(shader_name), shader_src(shader_text) {
}

void GLSLShader::loadFromFile(const char* filepath) {
	printf("(S) (%s) Reading from %s ... \n", getIDString().c_str(), _fullpath(NULL, filepath, 200));
	shader_src = "";
	std::ifstream fileStream(filepath, std::ios::in);

	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		shader_src.append(line + "\n");
	}

	fileStream.close();
}

std::string GLSLShader::getSrc() const {
	return shader_src;
}

void GLSLShader::setSrc(const char* new_source) {
	shader_src = std::string(new_source);
	compiled = false; // setting new source forces recompile
}

void GLSLShader::compile() {
	printf("(S) (%s) Compiling ... ", getIDString().c_str());
	shaderID = glCreateShader(shadertype);
	glShaderSource(shaderID, 1, ShaderStringHelper(shader_src), NULL);
	glCompileShader(shaderID);
	// check if shader compiled
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		getCompilationError(shaderID);
		glDeleteShader(shaderID);
		compiled = false;
	}
	else {
		compiled = true;
		printf("OK - ID: %i \n", shaderID);
	}
}

void GLSLShader::getCompilationError(GLuint shader) {
	int infologLength = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, (GLint *)&infologLength);
	char* infoLog = (char *)malloc(infologLength);
	glGetShaderInfoLog(shader, infologLength, NULL, infoLog); // will include terminate char
	printf("\n (S) (%s) Shader compilation error:\n%s\n", getIDString().c_str(), infoLog);
	free(infoLog);
}

std::string GLSLShader::getIDString() {
	if(shaderID == 0)
		return shader_name;
	else
		return shader_name + ":" + std::to_string(shaderID);
}