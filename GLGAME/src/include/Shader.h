#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

struct ShaderProgramSource {
	std::string vertexSource;
	std::string fragmentSource;
};

class Shader {
public:
	Shader(const std::string& filepath);
	~Shader();

	void bind() const;
	void unbind() const;

	//Set uniforms
	void setUniform1i(const std::string& name, int value);
	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void setUniformMat4f(const std::string& name, glm::mat4 matrix);

private:
	ShaderProgramSource parseShader(const std::string& filepath);
	unsigned int compileShader(const std::string& source, unsigned int type);
	unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);


	int getUniformLocation(const std::string& name) const;
	std::string m_filePath;
	unsigned int m_rendererID;
	mutable std::unordered_map<std::string, int> m_uniformLocationCache;
};