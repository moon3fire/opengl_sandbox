#include "ColorBatching.h"

#include <Renderer.h>
#include <imgui/imgui.h>

#define OBJECT_COUNT 2 // remember to add objectPositions when increasing this

namespace test {
	ColorBatching::ColorBatching() :m_proj(glm::ortho(0.0f, 1000.0f, 0.0f, 1000.0f, -1.0f, 1.0f)),
									m_view(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))) {

		float objectPositions1[] = {
			   -50.0f, -50.0f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f,
				50.0f, -50.0f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f,
				50.0f,  50.0f, 0.0f, 1.0f,  1.0f, 1.0f,  1.0f,
			   -50.0f,  50.0f, 0.0f, 1.0f,  1.0f, 1.0f,  1.0f 
		};

		float objectPositions2[] = {
				100.0f, 100.0f, 0.0f,  1.0f,  1.0f,  1.0f, 1.0f,
				200.0f, 100.0f, 0.0f,  1.0f,  1.0f,  1.0f, 1.0f,
				200.0f, 200.0f, 0.0f,  0.56f, 0.3f, 0.26f, 1.0f,
				100.0f, 200.0f, 0.0f,  0.56f, 0.3f, 0.26f, 1.0f,
		};

		std::vector<float*> positions;
		positions.push_back(objectPositions1);
		positions.push_back(objectPositions2);

		unsigned int indices[] = {
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4
		};

		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glEnable(GL_BLEND));

		VertexBufferLayout layouts[OBJECT_COUNT];

		for (int i = 0; i < OBJECT_COUNT; i++) {
			layouts[i].Push<float>(3);
			layouts[i].Push<float>(4);
			m_translations.push_back(glm::vec3(0, 0, 0));
			std::unique_ptr<VertexArray> va = std::make_unique<VertexArray>();
			m_VAOs.push_back(std::move(va));
			std::unique_ptr<VertexBuffer> vb = std::make_unique<VertexBuffer>(positions[i], 7 * 4 * sizeof(float));
			m_vertexBuffers.push_back(std::move(vb));
			m_VAOs[i]->addBuffer(*m_vertexBuffers[i], layouts[i]);
		}

		m_indexBuffer = std::make_unique<IndexBuffer>(indices, 12);

		m_shader = std::make_unique<Shader>("res/shaders/color_batching.shader");
		m_shader->bind();
	}

	ColorBatching::~ColorBatching() {}

	void ColorBatching::onUpdate(float deltaTime) {}

	void ColorBatching::onRender() {
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;

		for (int i = 0; i < OBJECT_COUNT; i++) {
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_translations[i]);
			glm::mat4 mvp = m_proj * m_view * model;

			m_shader->bind();
			m_shader->setUniformMat4f("u_MVP", mvp);
			renderer.draw(*m_VAOs[i], *m_indexBuffer, *m_shader);
		}
	}

	void ColorBatching::onImGuiRender() {
		for (int i = 0; i < OBJECT_COUNT; i++) {
			ImGui::SliderFloat3(std::string("Translation " + std::to_string(i)).c_str(), &m_translations[i].x, 0.0f, 1000.0f);
		}
	}

} // namespace test