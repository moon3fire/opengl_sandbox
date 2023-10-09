#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#include <GL/glew.h>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError(); \
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))    

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer {
public:
    void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    void clear() const;

private:

};