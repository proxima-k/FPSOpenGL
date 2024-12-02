
#pragma once

#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include <glew/glew.h>

class FrameBuffer {
public:
	FrameBuffer(unsigned int windowWidth, unsigned int windowHeight);
	~FrameBuffer();

	void Bind() const;
	void Unbind() const;

	unsigned int GetID() const { return rendererID; }
	unsigned int GetColorID()  const { return  colorTextureID; };
	unsigned int GetDepthID()  const { return  depthTextureID; };
	unsigned int GetNormalID() const { return normalTextureID; };

	static void CopyDepthBuffer(
		unsigned int source_FBO_ID, unsigned int target_FBO_ID,
		unsigned int windowWidth, unsigned int windowHeight);

private:
	unsigned int rendererID;

	// todo: extra thing to add could be width and height of the framebuffer

	// currently we don't have Texture class
	// so we have to manually create them here
	// these are attachments for the Framebuffer Object
	unsigned int colorTextureID;
	unsigned int depthTextureID;
	unsigned int normalTextureID;
};