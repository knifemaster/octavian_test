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

const float RADIUS = 0.5f;
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

	//GLFWwindow* window;

	if (!glfwInit()) {
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION, MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION, MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Octavian", nullptr, nullptr);

	if (!window) {
		std::cerr << "Failed to create GLFW window!" << std::endl;
		glfwTerminate();
		return -1;	
	}

	glfwMakeContextCurrent(window);

	if (!gladLoaderGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD!" << std::endl;
		return -1;
	}


	//if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
	//	std::cout << "couldn-t load opengl" << std::endl;
	//	glfwTerminate();
	//	return -1;
	//}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	generateCylinder(vertices, indices);

	unsigned int VAO, VBO, EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glVertextAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);





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
