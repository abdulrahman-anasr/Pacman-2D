#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <chrono>
#include <thread>

#include "Game.h"

const unsigned int SCR_WIDTH = 28 * 8 * 4, SCR_HEIGHT = 36 * 8 * 4;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void window_focus_callback(GLFWwindow* window, int focused);

GLFWwindow* window;

Game mainGame(SCR_WIDTH, SCR_HEIGHT);

int g_ScreenFPS = 60;

bool paused = false;

int main(int argc, char* argv)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Pac-Man", NULL, NULL);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (!window)
	{
		std::cout << "FAILED TO INITIALIZE WINDOW" << std::endl;
		return -220;
	}
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "FAILED TO INITIALIZE GLAD" << std::endl;
		return -221;
	}
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetWindowFocusCallback(window, window_focus_callback);

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float fpsLimit = 1.0f / 60.0f;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	

	

	mainGame.init();

	while (!glfwWindowShouldClose(window))
	{
		
		/*			DeltaTime Calculations		*/
		float currentTime = glfwGetTime();
		if (!paused)
		{
			deltaTime = currentTime - lastFrame;

			
		}
		else
		{
			deltaTime = 0.0f;
		}
		lastFrame = currentTime;

		mainGame.update(deltaTime);

		float averageFPS = 1.0 / deltaTime;

		mainGame.mAverageFrameRate = averageFPS;

		glfwPollEvents();
		mainGame.processInput(deltaTime);

		

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		/*	Rendering happens here*/	
		mainGame.render();
		
		
		glfwSwapBuffers(window);


	}

	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (key > 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			mainGame.mKeys[key] = true;
		}
		if (action == GLFW_RELEASE)
		{
			mainGame.mKeys[key] = false;
			mainGame.mKeysProcessed[key] = false;
			mainGame.mKeysProcessedFrames[key] = 0;
		}
	}
}

void window_focus_callback(GLFWwindow* window, int focused)
{
	if (focused)
	{
		paused = false;
	}
	else
	{
		paused = true;
	}
}