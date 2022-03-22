#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <iostream>

#include "shader.h"
#include "window.h"






float vertices[] =
{
	//TL
	//pos					//col			
	-1.0f, 0.0f, -1.0f,  	1.f, 0.0f, 0.0f,
	-1.0f, 0.0f, 1.0f,		1.f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,		1.f, 0.0f, 0.0f,

	//TB
	//pos					//col	
	-1.0f, 0.0f, -1.0f,  	0.f, 1.0f, 0.0f,
	1.0f, 0.0f, -1.0f,		0.f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,		0.f, 1.0f, 0.0f,

	//TR
	1.0f, 0.0f, -1.0f,  	1.f, 0.0f, 0.0f,
	1.0f, 0.0f, 1.0f,		1.f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,		1.f, 0.0f, 0.0f,

	//TF
	//pos					//col			
	-1.0f, 0.0f, 1.0f,  	0.f, 1.0f, 0.0f,
	1.0f, 0.0f, 1.0f,		0.f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,		0.f, 1.0f, 0.0f,


	//BL
	//pos					//col	
	-1.0f, 0.0f, 1.0f,  	0.f, 1.0f, 0.0f,
	-1.0f, 0.0f, -1.0f,		0.f, 1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,		0.f, 1.0f, 0.0f,

	//BB
	//pos					//col	
	-1.0f, 0.0f, -1.0f,  	1.f, 0.0f, 0.0f,
	1.0f, 0.0f, -1.0f,		1.f, 0.0f, 0.0f,
	0.0f, -1.0f, 0.0f,		1.f, 0.0f, 0.0f,

	//BR
	//pos					//col			
	1.0f, 0.0f, 1.0f,  	0.f, 1.0f, 0.0f,
	1.0f, 0.0f, -1.0f,		0.f, 1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,		0.f, 1.0f, 0.0f,

	//BF
	//pos					//col		
	-1.0f, 0.0f, 1.0f,  	1.f, 0.0f, 0.0f,
	1.0f, 0.0f, 1.0f,		1.f, 0.0f, 0.0f,
	0.0f, -1.0f, 0.0f,		1.f, 0.0f, 0.0f,
};

void processKeyboard(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)		glfwSetWindowShouldClose(window, true);
}

//DECLARE POSITION VARIABLES HERE
glm::vec3 oct_pos = glm::vec3(0.f, 0.f, -5.0f); // Octahedron world position

glm::vec3 camera_pos = glm::vec3(0.f, 0.5f, 0.f); // Camera position

int main(int argc, char** argv)
{
	GLFWwindow* window = CreateWindow(800, 600, "3D Modelling");

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	unsigned int shaderProgram = LoadShader("mvp.vert", "col.frag");

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

	//ENABLE DEPTH TEST HERE
	glEnable(GL_DEPTH_TEST);

	glUseProgram(shaderProgram);


	while (!glfwWindowShouldClose(window))
	{
		processKeyboard(window);

		glClearColor(0.f, 0.f, 0.f, 1.f);
		//CLEAR DEPTH BUFFER HERE
		//	|
		//	V
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


		//SET UP AND COPY MODEL MATRIX HERE
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, oct_pos);
		model = glm::rotate(model, (float)glfwGetTime() / 2, glm::vec3(0.f, 1.f, 0.f));
		int mloc = glGetUniformLocation(shaderProgram, "model");
		glUniformMatrix4fv(mloc, 1, GL_FALSE, glm::value_ptr(model));

		//SET UP AND COPY VIEW MATRIX HERE
		glm::mat4 view = glm::mat4(1.f);
		view = glm::translate(view, -camera_pos);
		int vloc = glGetUniformLocation(shaderProgram, "view");
		glUniformMatrix4fv(vloc, 1, GL_FALSE, glm::value_ptr(view));

		//SET UP AND COPY PROJECTION MATRIX HERE
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.f), (float)800 / (float)600, 1.0f, 10.0f);
		int ploc = glGetUniformLocation(shaderProgram, "projection");
		glUniformMatrix4fv(ploc, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 24);

		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}
