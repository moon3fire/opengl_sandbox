#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>

#include <tests/ClearColor/TestClearColor.h>
#include <tests/Texture/TestTexture.h>
#include <tests/ColorBatching/ColorBatching.h>

int main(void) {
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1000, 1000, "GLGAME", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		std::cout << "GLEW ERROR" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;
	{
		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, true);
		ImGui::StyleColorsDark();

		test::Test* currentTest = nullptr;
		test::TestMenu* testMenu = new test::TestMenu(currentTest);
		currentTest = testMenu;

		testMenu->RegisterTest<test::TestClearColor>("Clear color");
		testMenu->RegisterTest<test::TestTexture2D>("2D Texture");
		testMenu->RegisterTest<test::ColorBatching>("Color batching");

		Renderer renderer;

		while (!glfwWindowShouldClose(window)) {
			GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
			renderer.clear();

			ImGui_ImplGlfwGL3_NewFrame();
			if (currentTest) {
				currentTest->onUpdate(0.0f);
				currentTest->onRender();
				ImGui::Begin("Test");
				if (currentTest != testMenu && ImGui::Button("<-")) {
					delete(currentTest);
					currentTest = testMenu;
				}
				currentTest->onImGuiRender();
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				ImGui::End();
			}

			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
		
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		delete currentTest;
		if (currentTest != testMenu)
			delete testMenu;
	}
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}