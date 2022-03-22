#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <iostream>

#include "window.h"
#include "shader.h"

float move_x = 0;
float move_y = 0;

const float move_by = 0.001;

void processInput(GLFWwindow* window)
{
	glfwPollEvents();

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		move_y += move_by;
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		move_y -= move_by;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		move_x -= move_by;
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		move_x += move_by;
	}
}

unsigned int loadShaderSource()
{
	return 0;
}

int main(int argc, char** argv){
	GLFWwindow* window = CreateWindow(800, 600, "Danny's Program");

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	unsigned int shaderProgram = LoadShader("triangle.vert", "flat.frag");

	float vertices[] =
	{
		// Position			// Colour					
		0.5f, 0.5f, 0.f,	1.f, 0.f, 0.f,
		-0.5f, 0.5f, 0.f,	1.f, 0.f, 0.f,
		-0.f,  -0.5f, 0.f,	0.f, 0.f, 1.f
	};

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

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(1.f, 1.f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glUseProgram(shaderProgram);

		int ms = (int)(glfwGetTime() * 1000.f);
		float col_com = (float)(ms % 1000) / 1000.f;

		int loc = glGetUniformLocation(shaderProgram, "grey_level");
		glUniform1f(loc, col_com);

		int hori = glGetUniformLocation(shaderProgram, "move_x");
		glUniform1f(hori, move_x);

		int vert = glGetUniformLocation(shaderProgram, "move_y");
		glUniform1f(vert, move_y);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}

