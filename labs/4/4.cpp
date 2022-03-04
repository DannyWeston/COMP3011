#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <iostream>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "window.h"


float vertices0[] =
{
	//pos							col
	0.f, 0.25f, 0.f,  				1.f, 0.f, 0.f,	//t
	-0.25f, -0.25f, 0.f,  			1.f, 0.f, 0.f,	//bl
	0.25f,  -0.25f, 0.f,  			1.f, 0.f, 0.f	//br
};

float vertices1[] =
{
	//pos							col
	0.f, 0.25f, 0.f,  				0.f, 1.f, 0.f,	//t
	-0.25f, -0.25f, 0.f,  			0.f, 1.f, 0.f,	//bl
	0.25f,  -0.25f, 0.f,  			0.f, 1.f, 0.f	//br
};

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

int main(int argc, char** argv)
{
	GLFWwindow* window = CreateWindow(800, 600, "2D Transformations");


	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	unsigned int shader_program = LoadShader("shader.vert", "shader.frag");

	unsigned int VAO[2]; // VAOs
	glGenVertexArrays(2, VAO);

	unsigned int VBO[2]; // VBOs
	glGenBuffers(2, VBO);

	// Triangle 1

	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices0), vertices0, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Triangle 2

	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);



	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//

	glUseProgram(shader_program);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(1.f, 1.f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		/*
		Task 2:

		1, 0, 0, 0.5
		0, 1, 0, 0
		0, 0, 1, 0
		0, 0, 0, 1

		x

		0
		0.25
		0
		1

		=

		0 * 1 + 0.25 * 0 + 0 * 0.25 + 0.5 * 1 = 0.5
		0 * 0 + 1 * 0.25 + 0 * 0 + 0 * 1 = 0.25
		0 * 0 + 0 * 0.25 + 1 * 0 + 0 * 1 = 0
		0 * 0 + 0 * 0.25 + 0 * 0 + 0 * 1 * 1 = 1

		= 

		0.5
		0.25
		0
		1
		
		*/

		glm::mat4 M = glm::mat4(1.f);
		M = glm::translate(M, glm::vec3(0.5f, 0.0f, 0.0f));
		unsigned int loc = glGetUniformLocation(shader_program, "transformation");
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(M));
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glm::mat4 M2 = glm::mat4(1.f);
		M2 = glm::translate(M2, glm::vec3(0.0f, 0.5f, 0.0f));
		loc = glGetUniformLocation(shader_program, "transformation");
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(M2));
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}
