#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <iostream>

#include "ModelViewerCamera.h"
#include "camera.h"
#include "shader.h"
#include "window.h"

glm::vec3 cube_pos = glm::vec3(0.0f, 0.0f, 0.0f);

float vertices[] =
{
	//pos					//col			
	-0.5f, -0.5f, -0.5f,  	1.f, 0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,  	1.f, 0.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  	1.f, 0.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  	1.f, 0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  	1.f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  	1.f, 0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  	0.0f, 1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  	0.0f, 1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  	0.0f, 1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  	0.0f, 1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  	0.0f, 1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  	0.0f, 1.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  	0.0f, 0.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  	0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  	0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  	0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  	0.0f, 0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  	0.0f, 0.0f, 1.0f,

	0.5f,  0.5f,  0.5f,  	1.f, 1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  	1.f, 1.0f, 0.0f,
	0.5f, -0.5f, -0.5f, 	1.f, 1.0f, 0.0f,
	0.5f, -0.5f, -0.5f,  	1.f, 1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  	1.f, 1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  	1.f, 1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  	1.f, 0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  	1.f, 0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  	1.f, 0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  	1.f, 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  	1.f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  	1.f, 0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  	0.0f, 1.f, 1.0f,
	0.5f,  0.5f, -0.5f,  	0.0f, 1.f, 1.0f,
	0.5f,  0.5f,  0.5f,  	0.0f, 1.f, 1.0f,
	0.5f,  0.5f,  0.5f,  	0.0f, 1.f, 1.0f,
	-0.5f,  0.5f,  0.5f,  	0.0f, 1.f, 1.0f,
	-0.5f,  0.5f, -0.5f, 	0.0f, 1.f, 1.0f,
};

SCamera Camera;

void processKeyboard(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	bool updated = false;
	float x = 0.f, y = 0.f;

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		x = 1.f;
		y = 0.f;
		updated = true;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		x = -1.f;
		y = 0.f;
		updated = true;
	}
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		x = 0.f;
		y = 1.f;
		updated = true;
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		x = 0.f;
		y = -1.f;
		updated = true;
	}
	else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		cam_dist += 0.001;
		updated = true;
	}
	else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		cam_dist -= 0.001;
		updated = true;
	}

	if (updated) {
		MoveAndOrientCamera(Camera, cube_pos, cam_dist, x, y);
	}

}

int main(int argc, char** argv)
{
	GLFWwindow* window = CreateWindow(800, 600, "Model Viewer Camera");

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	unsigned int shaderProgram = LoadShader("mvp.vert", "col.frag");

	InitCamera(Camera);
	MoveAndOrientCamera(Camera, cube_pos, cam_dist, 0.f, 0.f);

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

		glClearColor(.0f, .0f, .0f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


		glm::mat4 model = glm::mat4(1.f);
		model = glm::translate(model, cube_pos);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));


		glm::mat4 view = glm::mat4(1.f);
		view = glm::lookAt(Camera.Position, Camera.Position + Camera.Front, Camera.Up);
		//glm::translate(view, -glm::vec3(0.f, 0.f, 3.f));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

		glm::mat4 projection = glm::mat4(1.f);
		projection = glm::perspective(glm::radians(45.f), (float)800 / (float)600, 1.f, 10.f);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}