#ifndef GLEW_STATIC
	#define GLEW_STATIC
#endif

// OpenGL
#include <GL/glew.h> // Take care: GLEW should be included before GLFW
#include <GLFW/glfw3.h>
// C++ libs
#include <string>
#include "shader_tools/GLSLProgram.h"
#include "shader_tools/GLSLShader.h"
#include "gl_tools.h"
#include "glfw_tools.h"
#include "gl_texture_tools.h"

using namespace std;

// GLFW
GLFWwindow* window;
int WIDTH = 512;
int HEIGHT = 512;

// OpenGL
GLuint VBO, VAO, EBO;
GLSLShader drawtex_f; // GLSL fragment shader
GLSLShader drawtex_v; // GLSL fragment shader
GLSLProgram shdrawtex; // GLSLS program for textured draw

// Regular OpenGL Texture
GLuint texture0;

// Shaders from CUDA2GL sample
static const char *glsl_drawtex_vertshader_src =
"#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec3 color;\n"
"layout (location = 2) in vec2 texCoord;\n"
"\n"
"out vec3 ourColor;\n"
"out vec2 ourTexCoord;\n"
"\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(position, 1.0f);\n"
"	ourColor = color;\n"
"	ourTexCoord = texCoord;\n"
"}\n";

static const char *glsl_drawtex_fragshader_src =
"#version 330 core\n"
"uniform sampler2D tex;\n"
"in vec3 ourColor;\n"
"in vec2 ourTexCoord;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"   	color = texture(tex, ourTexCoord);\n"
"}\n";

// QUAD GEOMETRY
GLfloat vertices[] = {
	// Positions          // Colors           // Texture Coords
	0.75f, 0.75f, 0.75f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // Top Right
	0.75f, -0.75f, 0.75f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // Bottom Right
	-0.75f, -0.75f, 0.75f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // Bottom Left
	-0.75f, 0.75f, 0.75f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f // Top Left 
};
// you can also put positions, colors and coordinates in seperate VBO's
GLuint indices[] = {  // Note that we start from 0!
	0, 1, 3,  // First Triangle
	1, 2, 3   // Second Triangle
};


void initGLBuffers()
{
	// create shader program
	drawtex_v = GLSLShader("Textured draw vertex shader", glsl_drawtex_vertshader_src, GL_VERTEX_SHADER);
	drawtex_f = GLSLShader("Textured draw fragment shader", glsl_drawtex_fragshader_src, GL_FRAGMENT_SHADER);
	shdrawtex = GLSLProgram(&drawtex_v, &drawtex_f);
	shdrawtex.compile();	
	// TODO: check error function
}

// Keyboard
void keyboardfunc(GLFWwindow* window, int key, int scancode, int action, int mods){
}

bool initGL(){
	glewExperimental = GL_TRUE; // need this to enforce core profile
	GLenum err = glewInit();
	glGetError(); // parse first error
	if (err != GLEW_OK) {// Problem: glewInit failed, something is seriously wrong.
		printf("glewInit failed: %s /n", glewGetErrorString(err));
		exit(1);
	}
	glViewport(0, 0, WIDTH, HEIGHT); // viewport for x,y to normalized device coordinates transformation
	// TODO: check error function
	return true;
}


bool initGLFW(){
	if (!glfwInit()) exit(1);
	// These hints switch the OpenGL profile to core
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(WIDTH, WIDTH, "The Simplest OpenGL Quad", NULL, NULL);
	if (!window){ glfwTerminate(); exit(1); }
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetKeyCallback(window, keyboardfunc);
	return true;
}

void display(void) {
	glfwPollEvents(); // Process events
	
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Clear the colorbuffer
	glClear(GL_COLOR_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture0);
	shdrawtex.use(); // we gonna use this compiled GLSL program
	glUniform1i(glGetUniformLocation(shdrawtex.program, "tex"), 0);

	glBindVertexArray(VAO); // binding VAO automatically binds EBO
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0); // unbind VAO

	// TODO: check error function
	
	// Swap the screen buffers
	glfwSwapBuffers(window);
}

int main(int argc, char *argv[]) {
	initGLFW();
	initGL();

	printGLFWInfo(window);
	printGlewInfo();
	printGLInfo();

	initGLBuffers();

	stbi_set_flip_vertically_on_load(true);
	texture0 = loadTextureFromFile(std::string("texture.jpg"));
	
	// Generate buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Buffer setup
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO); // all next calls wil use this VAO (descriptor for VBO)

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute (3 floats)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Color attribute (3 floats)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Texture attribute (2 floats)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound 
	// vertex buffer object so afterwards we can safely unbind
	glBindVertexArray(0);

	// Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
	// A VAO stores the glBindBuffer calls when the target is GL_ELEMENT_ARRAY_BUFFER. 
	// This also means it stores its unbind calls so make sure you don't unbind the element array buffer before unbinding your VAO, otherwise it doesn't have an EBO configured.

	while (!glfwWindowShouldClose(window))
	{
		display();
		glfwWaitEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(0);
}