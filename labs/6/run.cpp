#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <iostream>

#include "shader.h"
#include "window.h"

unsigned const int SCREEN_WIDTH = 800;
unsigned const int SCREEN_HEIGHT = 600;

glm::vec3 cube1_pos = glm::vec3(7.f, 0.f, 5.f);
glm::vec3 cube2_pos = glm::vec3(5.f, 0.f, 7.f);

glm::vec3 cam_pos = -glm::vec3(5.f, 5.f, 5.f);
glm::vec3 cam_rot = glm::vec3(1.0f, 0.0f, 0.0f);

float vertices[] = {
	//pos					//col			
	-1.0f, -1.0f, -1.0f,  	1.f, 0.0f, 0.0f,
	1.0f, -1.0f, -1.0f,  	1.f, 0.0f, 0.0f,
	1.0f,  1.0f, -1.0f,  	1.f, 0.0f, 0.0f,
	1.0f,  1.0f, -1.0f,  	1.f, 0.0f, 0.0f,
	-1.0f,  1.0f, -1.0f,  	1.f, 0.0f, 0.0f,
	-1.0f, -1.0f, -1.0f,  	1.f, 0.0f, 0.0f,

	-1.0f, -1.0f,  1.0f,  	0.0f, 1.0f, 0.0f,
	1.0f, -1.0f,  1.0f,  	0.0f, 1.0f, 0.0f,
	1.0f,  1.0f,  1.0f,  	0.0f, 1.0f, 0.0f,
	1.0f,  1.0f,  1.0f,  	0.0f, 1.0f, 0.0f,
	-1.0f,  1.0f,  1.0f,  	0.0f, 1.0f, 0.0f,
	-1.0f, -1.0f,  1.0f,  	0.0f, 1.0f, 0.0f,

	-1.0f,  1.0f,  1.0f,  	0.0f, 0.0f, 1.0f,
	-1.0f,  1.0f, -1.0f,  	0.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, -1.0f,  	0.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, -1.0f,  	0.0f, 0.0f, 1.0f,
	-1.0f, -1.0f,  1.0f,  	0.0f, 0.0f, 1.0f,
	-1.0f,  1.0f,  1.0f,  	0.0f, 0.0f, 1.0f,

	1.0f,  1.0f,  1.0f,  	1.f, 1.0f, 0.0f,
	1.0f,  1.0f, -1.0f,  	1.f, 1.0f, 0.0f,
	1.0f, -1.0f, -1.0f, 	1.f, 1.0f, 0.0f,
	1.0f, -1.0f, -1.0f,  	1.f, 1.0f, 0.0f,
	1.0f, -1.0f,  1.0f,  	1.f, 1.0f, 0.0f,
	1.0f,  1.0f,  1.0f,  	1.f, 1.0f, 0.0f,

	-1.0f, -1.0f, -1.0f,  	1.f, 0.0f, 1.0f,
	1.0f, -1.0f, -1.0f,  	1.f, 0.0f, 1.0f,
	1.0f, -1.0f,  1.0f,  	1.f, 0.0f, 1.0f,
	1.0f, -1.0f,  1.0f,  	1.f, 0.0f, 1.0f,
	-1.0f, -1.0f,  1.0f,  	1.f, 0.0f, 1.0f,
	-1.0f, -1.0f, -1.0f,  	1.f, 0.0f, 1.0f,

	-1.0f,  1.0f, -1.0f,  	0.0f, 1.f, 1.0f,
	1.0f,  1.0f, -1.0f,  	0.0f, 1.f, 1.0f,
	1.0f,  1.0f,  1.0f,  	0.0f, 1.f, 1.0f,
	1.0f,  1.0f,  1.0f,  	0.0f, 1.f, 1.0f,
	-1.0f,  1.0f,  1.0f,  	0.0f, 1.f, 1.0f,
	-1.0f,  1.0f, -1.0f, 	0.0f, 1.f, 1.0f,
};

void processKeyboard(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)		glfwSetWindowShouldClose(window, true);
}

int main(int argc, char** argv)
{
	GLFWwindow* window = CreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Lab 6a");

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	unsigned int shaderProgram = LoadShader("mvp.vert", "uniform.frag");

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//

	glEnable(GL_DEPTH_TEST);

	glUseProgram(shaderProgram);


	while (!glfwWindowShouldClose(window))
	{
		processKeyboard(window);

		glClearColor(1.f, 1.f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glm::mat4 view = glm::mat4(1.f);
		view = glm::rotate(view, -glm::radians(-90.f), cam_rot);
		view = glm::translate(view, cam_pos);

		int vloc = glGetUniformLocation(shaderProgram, "view");
		glUniformMatrix4fv(vloc, 1, GL_FALSE, glm::value_ptr(view));

		glm::mat4 projection = glm::mat4(1.f);
		projection = glm::perspective(glm::radians(45.f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 1.f, 10.f);
		int ploc = glGetUniformLocation(shaderProgram, "projection");
		glUniformMatrix4fv(ploc, 1, GL_FALSE, glm::value_ptr(projection));

		// Cube 1
		int cloc = glGetUniformLocation(shaderProgram, "uCol");
		glUniform3f(cloc, 1.f, 0.f, 0.f);

		glm::mat4 model = glm::mat4(1.f);
		model = glm::translate(model, cube1_pos);

		int mloc = glGetUniformLocation(shaderProgram, "model");
		glUniformMatrix4fv(mloc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Cube 2
		cloc = glGetUniformLocation(shaderProgram, "uCol");
		glUniform3f(cloc, 0.f, 1.f, 0.f);

		model = glm::mat4(1.f);
		model = glm::translate(model, cube2_pos);

		mloc = glGetUniformLocation(shaderProgram, "model");
		glUniformMatrix4fv(mloc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//

		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}