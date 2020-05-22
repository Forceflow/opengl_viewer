#pragma once

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <GL/glew.h>
#include <cstdio>
#include <string>
#include <fstream>
#include <exception>

// Helper to switch between C++ Strings and GLChar* references 
struct ShaderStringHelper{
	const char *p;
	ShaderStringHelper(const std::string& s) : p(s.c_str()) {}
	operator const char**() { return &p; }
};

// Function to load text from file
inline static std::string loadFileToString(const char *filename){
	std::ifstream file(filename, std::ios::in);
	std::string text;
	if (file){
		file.seekg(0, std::ios::end); // go to end
		text.resize(file.tellg()); // resize text buffer to file size
		file.seekg(0, std::ios::beg); // back to begin
		file.read(&text[0], text.size()); // read into buffer
		file.close();
	}
	else {
		std::string error_message = std::string("File not found: ") + std::string(_fullpath(NULL, filename, 200));
		fprintf(stderr, error_message.c_str());
		throw std::runtime_error(error_message);
	}
	return text;
}