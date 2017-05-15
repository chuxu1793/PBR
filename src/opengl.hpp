/*
 * Physically Based Rendering
 * Copyright (c) 2017 Michał Siejak
 */

#pragma once

#include <string>
#include <glad/glad.h>

#include "common/renderer.hpp"

namespace OpenGL {

struct VertexBuffer
{
	VertexBuffer() : vbo(0), ibo(0), vao(0) {}
	GLuint vbo, ibo, vao;
	GLuint numElements;
};

struct FrameBuffer
{
	FrameBuffer() : id(0), colorTarget(0), depthStencilTarget(0) {}
	GLuint id;
	GLuint colorTarget;
	GLuint depthStencilTarget;
	int width, height;
	int samples;
};

struct Texture
{
	Texture() : id(0) {}
	GLuint id;
	int width, height;
	int levels;
};

class Renderer final : public RendererInterface
{
public:
	GLFWwindow* initialize(int width, int height, int samples) override;
	void shutdown() override;
	void setup() override;
	void render(GLFWwindow* window, const ViewSettings& view) override;

private:
	static GLuint compileShader(const std::string& filename, GLenum type);
	static GLuint linkProgram(std::initializer_list<GLuint> shaders);

	static Texture createTexture(int width, int height, GLenum internalformat, int levels=0);
	static Texture createTexture(const std::shared_ptr<class Image>& image, GLenum format, GLenum internalformat, int levels=0);
	static void generateTextureMipmaps(const Texture& texture);
	static void deleteTexture(Texture& texture);

	static FrameBuffer createFrameBuffer(int width, int height, int samples, GLenum colorFormat, GLenum depthstencilFormat);
	static void resolveFramebuffer(const FrameBuffer& srcfb, const FrameBuffer& dstfb);
	static void deleteFrameBuffer(FrameBuffer& fb);

	static VertexBuffer createVertexBuffer(const std::shared_ptr<class Mesh>& mesh);
	static void deleteVertexBuffer(VertexBuffer& buffer);

	static VertexBuffer createClipSpaceQuad();

#if _DEBUG
	static void logMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
#endif

	FrameBuffer m_framebuffer;
	FrameBuffer m_resolveFramebuffer;

	VertexBuffer m_screenQuad;
	VertexBuffer m_skybox;
	VertexBuffer m_pbrModel;

	GLuint m_tonemapProgram;
	GLuint m_skyboxProgram;
	GLuint m_pbrProgram;

	Texture m_envTexture;

	Texture m_albedoTexture;
	Texture m_normalTexture;
	Texture m_metalnessTexture;
	Texture m_roughnessTexture;
};

} // OpenGL