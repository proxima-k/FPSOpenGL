#include "Renderer.h"
#include <iostream>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << std::endl;
        std::cout << "[OpenGL error] (" << error << "): " << std::endl;
        std::cout << ">>  function:  " << function << std::endl;
        std::cout << ">>  file:      " << file << ":" << line << std::endl;
        std::cout << ">>  line:      " << line << std::endl;
        std::cout << std::endl;
        return false;
    }
    return true;
}

void Renderer::Clear() const
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader) const
{
    shader.Bind();
    //shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

    vertexArray.Bind();
    indexBuffer.Bind();

    GLCall(glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr));
}
