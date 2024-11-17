#pragma once

#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

class OutlinePostProcess {
public:
	// essentially you give the class the data it needs and call render and it will render it to a quad of your choice
	OutlinePostProcess();
	~OutlinePostProcess();

	void render() const;
	
	void setColorTextureID (unsigned int ID) { colorTextureID  = ID; }
	void setDepthTextureID (unsigned int ID) { depthTextureID  = ID; }
	void setNormalTextureID(unsigned int ID) { normalTextureID = ID; }

private:

	unsigned int colorTextureID	 = 0;
	unsigned int depthTextureID	 = 0;
	unsigned int normalTextureID = 0;
	
	// note: can be changed to a quad class or "RenderTexture" like in Unity
	VertexArray* quadVAO = nullptr;
	VertexBuffer* quadVBO = nullptr;

	Shader* shader = nullptr;		// outline post processing shader
};