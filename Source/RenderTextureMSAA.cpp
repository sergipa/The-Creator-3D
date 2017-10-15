#include "RenderTextureMSAA.h"
#include "Application.h"
#include "ModuleWindow.h"

RenderTextureMSAA::RenderTextureMSAA()
{
	fboId = 0;
	fboMsaaId = 0;
	textureId = 0;
	rboId = 0;
	rboColorId = 0;
	rboDepthId = 0;
}

RenderTextureMSAA::~RenderTextureMSAA()
{
}

bool RenderTextureMSAA::Create(uint width, uint height, int MSAA_level)
{
	max_msaa_samples = 0;
	glGetIntegerv(GL_MAX_SAMPLES, &max_msaa_samples);
	if (MSAA_level > max_msaa_samples) MSAA_level = max_msaa_samples;
	current_msaa_samples = MSAA_level;

	// create a texture object
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// create a MSAA framebuffer object
	glGenFramebuffers(1, &fboMsaaId);
	glBindFramebuffer(GL_FRAMEBUFFER, fboMsaaId);

	// create a MSAA renderbuffer object to store color info
	glGenRenderbuffers(1, &rboColorId);
	glBindRenderbuffer(GL_RENDERBUFFER, rboColorId);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, MSAA_level, GL_RGB8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// create a MSAA renderbuffer object to store depth info
	glGenRenderbuffers(1, &rboDepthId);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepthId);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, MSAA_level, GL_DEPTH_COMPONENT, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// attach msaa RBOs to FBO attachment points
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rboColorId);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepthId);

	// create a normal (no MSAA) FBO to hold a render-to-texture
	glGenFramebuffers(1, &fboId);
	glBindFramebuffer(GL_FRAMEBUFFER, fboId);

	glGenRenderbuffers(1, &rboId);
	glBindRenderbuffer(GL_RENDERBUFFER, rboId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// attach a texture to FBO color attachement point
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);

	// attach a rbo to FBO depth attachement point
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboId);

	// check FBO status
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	this->width = width;
	this->height = height;

	return true;
}

void RenderTextureMSAA::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fboMsaaId);
	glViewport(0, 0, width, height);
}

void RenderTextureMSAA::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, App->window->GetWidth(), App->window->GetHeight());
}

void RenderTextureMSAA::Render()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fboMsaaId);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboId);
	glBlitFramebuffer(0, 0, width, height,  // src rect
		0, 0, width, height,  // dst rect
		GL_COLOR_BUFFER_BIT, // buffer mask
		GL_LINEAR); // scale filter
}

void RenderTextureMSAA::ChangeMSAALevel(int MSAA_level)
{
	Destroy();
	Create(width, height, MSAA_level);
}

void RenderTextureMSAA::Destroy()
{
	glDeleteTextures(1, &textureId);
	textureId = 0;

	glDeleteFramebuffers(1, &fboId);
	fboId = 0;
	glDeleteFramebuffers(1, &fboMsaaId);
	fboMsaaId = 0;

	glDeleteRenderbuffers(1, &rboId);
	glDeleteRenderbuffers(1, &rboColorId);
	glDeleteRenderbuffers(1, &rboDepthId);
	rboId = 0;
	rboColorId = 0;
	rboDepthId = 0;
}

uint RenderTextureMSAA::GetTexture() const
{
	return textureId;
}

int RenderTextureMSAA::GetMaxMSAALevel() const
{
	return max_msaa_samples;
}

int RenderTextureMSAA::GetCurrentMSAALevel() const
{
	return current_msaa_samples;
}
