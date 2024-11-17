#include "OutlinePostProcess.h"

#include <ShaderManager.h>
#include "VertexBufferLayout.h"

OutlinePostProcess::OutlinePostProcess()
{
    // setup post process components 
    float texVertices[] = {
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    quadVAO = new VertexArray();
    quadVBO = new VertexBuffer(texVertices, sizeof(texVertices));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    quadVAO->AddBuffer(*quadVBO, layout);

    shader = shaderManager->getShader("outlinePP");
}

OutlinePostProcess::~OutlinePostProcess()
{
    delete quadVAO;
    delete quadVBO;
}

void OutlinePostProcess::render() const
{
    // prevents the quad from registering it's depth values to the framebuffer
    glDisable(GL_DEPTH_TEST);
    
    // prepare all the data required for the shader to use
    shader->Bind();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colorTextureID);
    shader->SetInt("u_colorTexture", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthTextureID);
    shader->SetInt("u_depthTexture", 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, normalTextureID);
    shader->SetInt("u_normalTexture", 2);

    quadVAO->Bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // reenable depth testing so following rendered meshes will have depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}
