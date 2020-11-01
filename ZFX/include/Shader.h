#pragma once
#include "zfxdefs.h"
#include <string>
#include <glew/include/GL/glew.h>

namespace ZFX
{
	class Shader
	{
	public:
		Shader(const std::string& filename);
		Shader(const Shader& other) = delete;
		Shader& operator=(const Shader& other) = delete;
		~Shader();

		void bind();
		void update();

	protected:
		static void checkError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMsg);
		static std::string load(const std::string& fileName);
		static GLuint create(const std::string& text, GLenum shaderType);

	protected:
		enum
		{
			VERTEX_SHADER,
			FRAGMENT_SHADER,

			NUM_SHADERS
		};

		GLuint m_program;
		GLuint m_shaders[NUM_SHADERS];
		GLuint m_uniforms[NUM_UNIFORMS];
	};
}

