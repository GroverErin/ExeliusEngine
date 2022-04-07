#include "EXEPCH.h"
#include "OpenGLRendererAPI.h"
#include "source/render/VertexArray.h"
#include "source/render/Buffer.h"

#include <glad/glad.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	void OpenGLMessageCallback(
		GLenum,
		GLenum,
		GLuint,
		GLenum severity,
		GLsizei,
		const GLchar* message,
		const void*)
	{
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:
			{
				EXE_LOG_CATEGORY_FATAL("OpenGL", message);
				return;
			}
			case GL_DEBUG_SEVERITY_MEDIUM:
			{
				EXE_LOG_CATEGORY_ERROR("OpenGL", message);
				return;
			}
			case GL_DEBUG_SEVERITY_LOW:
			{
				EXE_LOG_CATEGORY_WARN("OpenGL", message);
				return;
			}
			case GL_DEBUG_SEVERITY_NOTIFICATION:
			{
				EXE_LOG_CATEGORY_TRACE("OpenGL", message);
				return;
			}
		}
		
		EXE_LOG_CATEGORY_FATAL("OpenGL", message);
		EXE_ASSERT(false);
	}

	void OpenGLRendererAPI::Initialize()
	{
#ifdef EXE_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback((GLDEBUGPROC)OpenGLMessageCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LINE_SMOOTH);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(Color color)
	{
		glm::vec4 colorVec = color.GetColorVector();
		glClearColor(colorVec.r, colorVec.g, colorVec.b, colorVec.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const SharedPtr<VertexArray>& vertexArray, uint32_t indexCount)
	{
		vertexArray->Bind();
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::DrawLines(const SharedPtr<VertexArray>& vertexArray, uint32_t vertexCount)
	{
		vertexArray->Bind();
		glDrawArrays(GL_LINES, 0, vertexCount);
	}

	void OpenGLRendererAPI::SetLineWidth(float width)
	{
		glLineWidth(width);
	}
}
