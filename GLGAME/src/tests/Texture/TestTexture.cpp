#include "TestTexture.h"

#include <Renderer.h>
#include <imgui/imgui.h>

namespace test {
	TestTexture2D::TestTexture2D() :m_translationA(0, 0, 0), m_translationB(0, 0, 0),
	m_proj(glm::ortho(0.0f, 1000.0f, 0.0f, 1000.0f, -1.0f, 1.0f)), m_view(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))) {
		
		float positions[] = {
			 -50.0f, -50.0f, 0.0f, 0.0f, //0
			  50.0f, -50.0f, 1.0f, 0.0f, //1
			  50.0f,  50.0f, 1.0f, 1.0f, //2
			 -50.0f,  50.0f, 0.0f, 1.0f  //3
		};

		unsigned int indices[] = {
			0, 1, 2, 
			2, 3, 0
		};
		
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glEnable(GL_BLEND));

		m_VAO = std::make_unique<VertexArray>();

 		m_vertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		m_VAO->addBuffer(*m_vertexBuffer, layout);

		m_indexBuffer = std::make_unique<IndexBuffer>(indices, 6);

		m_shader = std::make_unique<Shader>("res/shaders/texture.shader");
		m_shader->bind();
		//m_shader.setUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
		m_texture = std::make_unique<Texture>("res/textures/wood.png");
		m_shader->setUniform1i("u_Texture", 0);
	}

	TestTexture2D::~TestTexture2D() {}

	void TestTexture2D::onUpdate(float deltaTime) {}

	void TestTexture2D::onRender() {
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;

		m_texture->bind();

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_translationA);
			glm::mat4 mvp = m_proj * m_view * model;

			m_shader->bind();
			m_shader->setUniformMat4f("u_MVP", mvp);
			renderer.draw(*m_VAO, *m_indexBuffer, *m_shader);
		}

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_translationB);
			glm::mat4 mvp = m_proj * m_view * model;

			m_shader->bind();
			m_shader->setUniformMat4f("u_MVP", mvp);
			renderer.draw(*m_VAO, *m_indexBuffer, *m_shader);
		}
	}
	void TestTexture2D::onImGuiRender() {
		ImGui::SliderFloat3("TranslationA", &m_translationA.x, 0.0f, 1000.0f);
		ImGui::SliderFloat3("TranslationB", &m_translationB.x, 0.0f, 1000.0f);
	}
} // namespace test