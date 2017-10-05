#pragma once

#include <GL/glew.h>
#include <cstdio>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include "libs/stb_image.h"

GLuint loadTextureFromFile(std::string filepath) {
	GLuint texture0;
	glGenTextures(1, &texture0); // Load simple OpenGL texture
	glBindTexture(GL_TEXTURE_2D, texture0); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
											// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	printf("Loading texture from %s ... ", filepath.c_str());
	unsigned char *data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);
	if (data) {
		printf("OK - Texture ID: (%i) \n", texture0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else { printf("Error: No texture found. \n"); }
	stbi_image_free(data);
	return texture0;
}
