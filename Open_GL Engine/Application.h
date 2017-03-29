#pragma once
#include <glm\glm.hpp>

struct GLFWwindow;

class Application
{
public:
	Application();
	~Application();

	void run(const char* title, int width, int height, bool fullscreen);

	virtual bool startup() = 0;
	virtual void shutdown() = 0;
	virtual void update(float deltaTime) = 0;
	virtual void draw() = 0;

	// wipes the screen clear to begin a frame of drawing
	void clearScreen();

	// sets the colour that the sceen is cleared to
	void setBackgroundColour(float r, float g, float b, float a = 1.0f);
	void setBackgroundColour(glm::vec4 a_values);

	// access to the GLFW window
	GLFWwindow* getWindowPtr() const { return m_window; }

	float getTime()  const;

	unsigned int getWindowWidth() const;
	unsigned int getWindowHeight() const;



protected:
	virtual bool createWindow(const char* title, int width, int height, bool fullscreen);
	virtual void destroyWindow();

	GLFWwindow*		m_window;

	bool m_gameOver;

};

