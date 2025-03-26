#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

std::string readFile(const char* path) {
	std::string content, line;
	std::ifstream f(path);
	if (!f.is_open()) {
		std::cout << "Error: couldn't open " << path << std::endl;
		return "";
	}
	while (!f.eof()) {
		std::getline(f, line); content.append(line + "\n");
	}
	f.close(); return content;
}

// Vertex Shader source code
std::string vertShader = readFile("vert.glsl");
const char* vertexShaderSource = vertShader.c_str();
std::string fragShader = readFile("frag.glsl");
const char* fragmentShaderSource = fragShader.c_str();
int main()
{
	std::cout << vertexShaderSource;
	// Initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(
		800, 800, "Opengl-window", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate(); return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);
	gladLoadGL();
	// Utwórz obiekt Vertex Shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Połącz istniejący obiekt z napisaną wcześniej implementacją shadera
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// Skompiluj gotowy kod
	glCompileShader(vertexShader);
	// Powtórz operację dla fragment shadera
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);



	// Utwórz program
	GLuint shaderProgram = glCreateProgram();
	// Podłącz shadery pod program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Usuń niepotrzebne shadery
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLfloat floatroot3 = GLfloat(sqrt(3));
	// Vertices coordinates
	/*
	GLfloat vertices[] = {
	-0.5f, -0.5f * floatroot3 / 3, 0.0f, // Lower left corner
	0.5f, -0.5f * floatroot3 / 3, 0.0f, // Lower right corner
	0.0f, 0.5f * floatroot3 * 2 / 3, -0.5f * floatroot3 * 2 / 3, // Upper corner
	};*/
#define A 0.5f, 0.f, 0.5f, 1.f, 0.f, 0.f, 1.0f, 0.0f
#define B 0.5f, 0.f, -0.5f, 0.f, 1.f, 0.f, 0.0f, 0.0f
#define C -0.5f, 0.f, -0.5f,  1.f, 0.f, 1.f, 1.0f, 0.0f
#define D -0.5f, 0.f, 0.5f, 0.f, 1.f, 1.f, 0.0f, 0.0f
#define E 0.f, float(sqrt(2))/2, 0.f, 0.f, 0.f, 1.f, 0.5f, 1.0f
	GLfloat vertices[] = {
		A, B, E,
		B, C, E,
		C, D, E,
		A, D, E
	};
	
	unsigned int texture;
	glGenTextures(1, &texture); 
	glBindTexture(GL_TEXTURE_2D, texture);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *image_data = stbi_load("korwin.png", &width, &height, &nrChannels, 0); 
	if(image_data){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cout << "no image data\n";
	}
	stbi_image_free(image_data);

	GLuint VAO, VBO;
	// Utwórz obiekty VBO i VAO, każdy posiada jeden obiekt
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// Połącz wierzchołki z bufforem wierzchołków
	glBindVertexArray(VAO);
	// Ustaw typ VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// załaduj przygotowane wierzchołki
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
		GL_STATIC_DRAW);
	// Skonfiguruj format buffora, typ danych i długość
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 *
		sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 *
		sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2); 
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glClearDepth(1.f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glViewport(0, 0, 800, 800);
	// Specify the color of the background
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	// Clean the back buffer and assign the new color to it
	glClear(GL_COLOR_BUFFER_BIT);
	// Swap the back buffer with the front buffer
	glfwSwapBuffers(window);
	int timeLoc = glGetUniformLocation(shaderProgram, "time");
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 1);
	while (!glfwWindowShouldClose(window))
	{

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgram);

		float time = glfwGetTime();
		glUniform1f(timeLoc, time);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 12);
		// Odśwież widok
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}
