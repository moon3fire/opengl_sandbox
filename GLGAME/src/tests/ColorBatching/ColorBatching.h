#pragma once

#include <tests/Test.h>

#include <VertexBuffer.h>
#include <VertexBufferLayout.h>
#include <Texture.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <memory>

namespace test {

	class ColorBatching : public Test {
	public:
		ColorBatching();
		~ColorBatching();

		void onUpdate(float deltaTime) override;
		void onRender() override;
		void onImGuiRender() override;

	private:
		std::vector<std::unique_ptr<VertexArray>> m_VAOs;
		std::unique_ptr<IndexBuffer> m_indexBuffer;
		std::vector<std::unique_ptr<VertexBuffer>> m_vertexBuffers;
		std::unique_ptr<Shader> m_shader;

		glm::mat4 m_proj, m_view;
		std::vector<glm::vec3> m_translations;
	};

} // namespace test