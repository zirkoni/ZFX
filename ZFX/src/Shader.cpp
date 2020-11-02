#include "Shader.h"
#include <stdexcept>
#include <fstream>

ZFX::Shader::Shader(const std::string& filename) : m_program{ 0 }
{
	m_program = glCreateProgram();

	m_shaders[VERTEX_SHADER] = create(load(filename + ".vs"), GL_VERTEX_SHADER);
	m_shaders[FRAGMENT_SHADER] = create(load(filename + ".fs"), GL_FRAGMENT_SHADER);

	for (unsigned int i = 0; i < NUM_SHADERS; ++i)
	{
		glAttachShader(m_program, m_shaders[i]);
	}

	glBindAttribLocation(m_program, POSITION_VA, "positionIn");
	glBindAttribLocation(m_program, COLOUR_VA, "colourIn");

	glLinkProgram(m_program);
	checkError(m_program, GL_LINK_STATUS, true, "Error: glLinkProgram failed: ");

	glValidateProgram(m_program);
	checkError(m_program, GL_VALIDATE_STATUS, true, "Error: glValidateProgram failed: ");

	m_uniforms[TRANSFORM_U] = glGetUniformLocation(m_program, "transform");
}

ZFX::Shader::~Shader()
{
	for (uint32_t i = 0; i < NUM_SHADERS; i++)
	{
		glDetachShader(m_program, m_shaders[i]);
		glDeleteShader(m_shaders[i]);
	}

	glDeleteProgram(m_program);
}

void ZFX::Shader::bind()
{
	glUseProgram(m_program);
}

void ZFX::Shader::update()
{
	// TODO: MVP + Camera
}

void ZFX::Shader::checkError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMsg)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
	{
		glGetProgramiv(shader, flag, &success);
	} else
	{
		glGetShaderiv(shader, flag, &success);
	}

	if (success == GL_FALSE)
	{
		if (isProgram)
		{
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		} else
		{
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);
		}

		throw std::runtime_error{ errorMsg + ": " + error };
	}
}

std::string ZFX::Shader::load(const std::string& fileName)
{
	std::ifstream file(fileName.c_str());

	std::string output;
	std::string line;

	if (file)
	{
		while (std::getline(file, line))
		{
			output.append(line + "\n");
		}
		file.close();
	} else
	{
		throw std::runtime_error{ "Error: Unable to load shader: " + fileName };
	}

	return output;
}

GLuint ZFX::Shader::create(const std::string& text, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	if (0 == shader)
	{
		throw std::runtime_error{ "Error: glCreateShader failed, shader type " + shaderType };
	} else
	{
		const GLchar* p[1];
		p[0] = text.c_str();
		GLint lengths[1];
		lengths[0] = (GLint)text.length();

		glShaderSource(shader, 1, p, lengths);
		glCompileShader(shader);

		checkError(shader, GL_COMPILE_STATUS, false, "Error compiling shader!");
	}

	return shader;
}
