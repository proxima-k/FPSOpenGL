#pragma once

#include <string>
#include <unordered_map>
#include <glm/gtc/type_ptr.hpp>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;

public:
	Shader(const std::string& filePath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	unsigned int GetID() const;

	// Set uniforms
	void SetInt(const std::string& name, int value);
	void SetFloat(const std::string& name, float value);
	void SetFloat2(const std::string& name, float v0, float v1);
	void SetFloat3(const std::string& name, float v0, float v1, float v2);
	void SetFloat4(const std::string& name, float v0, float v1, float v2, float v3);
	void SetMat4(const std::string& name, glm::mat4 matrix4x4);


private:
	ShaderProgramSource ParseShader(const std::string& filePath);
	unsigned int CompileShader(unsigned int shaderType, const std::string& sourceCode);
	unsigned int CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);

	unsigned int GetUniformLocation(const std::string& name);
};