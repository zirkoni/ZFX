#pragma once
#include "zfxdefs.h"
#include <string>
#include <GL/glew.h>



namespace ZFX
{
	class Camera;
	class Transform;


	class ShaderBase
	{
	public:
		ShaderBase();
		ShaderBase(const ShaderBase& other) = delete;
		ShaderBase& operator=(const ShaderBase& other) = delete;
		virtual ~ShaderBase();

		/* Use this shader */
		void bind();

		/* Apply transformations */
		void update(const Transform& transform, const Camera& camera);

	protected:
		void createAndAttach(const std::string& filename);
		void compileAndSetUniforms();

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


	/* Basic shader */
	class Shader: public ShaderBase
	{
	public:
		Shader(const std::string& filename);
		Shader(const Shader& other) = delete;
		Shader& operator=(const Shader& other) = delete;
	};


	/* Shader with textures */
	class ShaderTex : public ShaderBase
	{
	public:
		ShaderTex(const std::string& filename);
		ShaderTex(const ShaderTex& other) = delete;
		ShaderTex& operator=(const ShaderTex& other) = delete;
	};
}

