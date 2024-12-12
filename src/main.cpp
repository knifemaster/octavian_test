#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <cmath>
//#define GLAD_GL_IMPLEMENTATION
//#include <glad/gl.h>
//#define GLFW_INCLUDE_NONE
//#include <GLFW/glfw3.h>

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

const int SEGMENTS = 50;

const float radius = 0.5f;
const float HEIGHT_CYLINDER = 1.0f;

const double PI  = 3.141592653589793238463;


void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}


void generateCylinder(std::vector<float>& vertices, std::vector<unsigned int>& indices) {

	for (int i = 0; i <= SEGMENTS; ++i) {
		float angle = 2.0f * PI * i / SEGMENTS;
		float x = RADIUS * std::cos(angle);
		float z = RADIUS * std::sin(angle);

		vertices.push_back(x);
		vertices.push_back(HEIGHT_CYLINDER / 2.0f);
		vertices.push_back(z);
		vertices.push_back(x);
		vertices.push_back(-HEIGHT_CYLINDER / 2.0f);
		vertices.push_back(z);
	}

	for (int i = 0; i < SEGMENTS; ++i) {
		indices.push_back(i * 2);
		indices.push_back(i * 2 + 1);
		indices.push_back((i * 2 + 2) % (SEGMENTS * 2));
		indices.push_back(i * 2 + 1);
		indices.push_back((i * 2 + 3) % (SEGMENTS * 2));
		indices.push_back((i * 2 + 2) % (SEGMENTS * 2));	

	
	}

}


int main() {

	GLFWwindow* window;

	if (!glfwInit()) {
		return -1;
	}

	window = glfwCreateWindow(640, 480, "Octavian", NULL, NULL);
	glfwMakeContextCurrent(window);

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "couldn-t load opengl" << std::endl;
		glfwTerminate();
		return -1;
	}

	glClearColor(0.10f, 0.25f, 0.50f, 1.0f);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
	}

	//std::cout << "Hello world" << std::endl;

	glfwTerminate();

	return 0;
}
