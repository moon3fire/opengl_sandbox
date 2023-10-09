#include "Renderer.h"

#include "Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include "GL/glew.h"

Shader::Shader(const std::string& filepath) :m_filePath(filepath), m_rendererID(0) {
    ShaderProgramSource source = parseShader(filepath);
    m_rendererID = createShader(source.vertexSource, source.fragmentSource);
}

Shader::~Shader() {
    GLCall(glDeleteProgram(m_rendererID));
}

void Shader::bind() const {
    GLCall(glUseProgram(m_rendererID));
}

void Shader::unbind() const {
    GLCall(glUseProgram(0));
}

void Shader::setUniform1i(const std::string& name, int value) {
    GLCall(glUniform1i(getUniformLocation(name), value));
}

void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
    GLCall(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
}

void Shader::setUniformMat4f(const std::string& name, glm::mat4 matrix) {
    GLCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::getUniformLocation(const std::string& name) const {
    if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end()) {
        return m_uniformLocationCache[name];
    }

    GLCall(int location = glGetUniformLocation(m_rendererID, name.c_str()));
    if (location == -1) {
        std::cout << "Warning: uniform " << name << " doesn't exist" << std::endl;
    }
    
    m_uniformLocationCache[name] = location;
    return location;
}

ShaderProgramSource Shader::parseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    ShaderType type = ShaderType::NONE;

    std::string line;
    std::stringstream ss[2];

    while (std::getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        }
        else {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::compileShader(const std::string& source, unsigned int type)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!" << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();

    unsigned int vs = compileShader(vertexShader, GL_VERTEX_SHADER);
    unsigned int fs = compileShader(fragmentShader, GL_FRAGMENT_SHADER);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

