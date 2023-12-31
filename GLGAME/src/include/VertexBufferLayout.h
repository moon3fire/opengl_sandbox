#pragma once

#include <GL/glew.h>

#include <vector>

#include "Renderer.h"

struct VertexBufferElement {
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int getSizeOfType(unsigned int type) {
		switch (type)
		{
			case GL_FLOAT: return 4;
			case GL_UNSIGNED_INT: return 4;
			case GL_UNSIGNED_BYTE: return 1;
		}
		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout {
public:
	VertexBufferLayout() :m_stride(0) {

	}

	template <typename T>
	void Push(unsigned int count);

	template <>
	void Push<float>(unsigned int count) {
		VertexBufferElement elem{ GL_FLOAT, count, GL_FALSE };
		m_elements.push_back(elem);
		m_stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
	}

	template <>
	void Push<unsigned int>(unsigned int count) {
		VertexBufferElement elem{ GL_UNSIGNED_INT, count, GL_FALSE };
		m_elements.push_back(elem);
		m_stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
	}

	template <>
	void Push<unsigned char>(unsigned int count) {
		VertexBufferElement elem{ GL_UNSIGNED_BYTE, count, GL_FALSE };
		m_elements.push_back(elem);
		m_stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
	}

	inline const std::vector<VertexBufferElement> getElements() const { return m_elements; }
	inline unsigned int getStride() const { return m_stride; }

private:
	std::vector<VertexBufferElement> m_elements;
	unsigned int m_stride;
};