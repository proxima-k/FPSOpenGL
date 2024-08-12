#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

Shader::Shader(const std::string& filePath)
	: m_FilePath(filePath), m_RendererID(0)
{
    ShaderProgramSource source = ParseShader(filePath);
    m_RendererID = CreateShaderProgram(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

ShaderProgramSource Shader::ParseShader(const std::string& filePath)
{
    std::ifstream stream(filePath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream stringStream[2];
    ShaderType shaderType = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                shaderType = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                shaderType = ShaderType::FRAGMENT;
        }
        else
        {
            stringStream[(int)shaderType] << line << '\n';
        }
    }

    return { stringStream[0].str(), stringStream[1].str() };
}

unsigned int Shader::CompileShader(unsigned int shaderType, const std::string& sourceCode)
{
    const unsigned int shaderID = glCreateShader(shaderType);
    const char* src = sourceCode.c_str();
    glShaderSource(shaderID, 1, &src, nullptr);
    glCompileShader(shaderID);

    int result;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(shaderID, length, &length, message);

        std::cout << "Failed to compile " <<
            (shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << " shader!" << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(shaderID);
        return 0;
    }

    return shaderID;
}


unsigned int Shader::CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int programID = glCreateProgram();
    unsigned int vertexShaderID = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fragmentShaderID = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);
    glValidateProgram(programID);

    // error check

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    return programID;
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

unsigned int Shader::GetID() const
{
    return m_RendererID;
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

unsigned int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
        std::cout << "[Shader warning] uniform '" << name << "' doesn't exist or not being used!\n"
                  << ">> shader file: " << m_FilePath << std::endl;
    
    m_UniformLocationCache[name] = location;
    return location;
}
