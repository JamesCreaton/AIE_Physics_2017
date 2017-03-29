#include "Application.h"
#include <gl_core_4_4.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>

#include "imgui_glfw3.h"
#include "Input.h"


Application::Application()
	: m_window(nullptr),
	m_gameOver(false) {
}


Application::~Application()
{
}

void Application::run(const char * title, int width, int height, bool fullscreen)
{
	if (createWindow(title, width, height, fullscreen) &&
		startup()) {

		//Variables for timing
		double prevTime = glfwGetTime();
		double currTime = 0;
		double deltaTime = 0;

		while (!m_gameOver)
		{
			// update delta time
			currTime = glfwGetTime();
			deltaTime = currTime - prevTime;
			prevTime = currTime;

			//Clear Input
			aie::Input::getInstance()->clearStatus();

			// update window events (input etc)
			glfwPollEvents();

			aie::ImGui_NewFrame();

			update(float(deltaTime));
			draw();

			ImGui::Render();

			glfwSwapBuffers(m_window);

			m_gameOver = m_gameOver || glfwWindowShouldClose(m_window) == GLFW_TRUE;
		}

	}

	// cleanup
	shutdown();
	destroyWindow();
}

float Application::getTime() const {
	return (float)glfwGetTime();
}

unsigned int Application::getWindowWidth() const {
	int w = 0, h = 0;
	glfwGetWindowSize(m_window, &w, &h);
	return w;
}

unsigned int Application::getWindowHeight() const {
	int w = 0, h = 0;
	glfwGetWindowSize(m_window, &w, &h);
	return h;
}

bool Application::createWindow(const char * title, int width, int height, bool fullscreen)
{

	if (glfwInit() == GL_FALSE)
		return false;

	m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (m_window == nullptr) {
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		glfwDestroyWindow(m_window);
		glfwTerminate();
		return false;
	}

	glClearColor(0, 0, 0, 1);

	glEnable(GL_DEPTH_TEST);

	// start input manager
	aie::Input::create();

	aie::ImGui_Init(m_window, true);

	return true;
}

void Application::destroyWindow(){
	aie::ImGui_Shutdown();
	aie::Input::destroy();

	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void Application::clearScreen(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Application::setBackgroundColour(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
}

void Application::setBackgroundColour(glm::vec4 a_values) {
	glClearColor(a_values.r, a_values.g, a_values.b, a_values.a);
}



