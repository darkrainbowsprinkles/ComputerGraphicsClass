#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getmuevex() { return muevex; }
	GLfloat getmuevex2() { return muevex2; }
	bool getLuzLampara() { return luzLampara; }
	int getFaroColorState() { return faroColorState; }
	bool getNaveAvanzando() { return naveAvanzando; }
	bool getNaveRetrocediendo() { return naveRetrocediendo; }
	GLfloat getPezPosX() { return pezPosX; }
	GLfloat getPezPosY() { return pezPosY; }
	GLfloat getPezPosZ() { return pezPosZ; }
	bool getLuzFocoPez() { return luzFocoPez; }
	GLfloat getPezSpotDirX() { return pezSpotDirX; }
	GLfloat getPezSpotDirY() { return pezSpotDirY; }
	GLfloat getPezSpotDirZ() { return pezSpotDirZ; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	
	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLfloat muevex;
	GLfloat muevex2;
	GLfloat pezPosX;
	GLfloat pezPosY;
	GLfloat pezPosZ;
	GLfloat pezSpotDirX;
	GLfloat pezSpotDirY;
	GLfloat pezSpotDirZ;
	bool luzLampara;
	bool luzFocoPez;
	int faroColorState;
	bool naveAvanzando;
	bool naveRetrocediendo;
	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};

