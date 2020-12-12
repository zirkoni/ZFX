#include "Shader.h"
#include "Camera.h"
#include "Transform.h"
#include <stdexcept>
#include <fstream>

const std::string ZFX::TRANSFORM_UNIFORM = "transform";
const ZFX::Uniforms ZFX::DEFAULT_UNIFORMS = { ZFX::TRANSFORM_UNIFORM };

ZFX::Shader::Shader(const std::string& filename, const VertexAttributes& attributes):
    Shader{ filename, attributes, DEFAULT_UNIFORMS }
{
}

ZFX::Shader::Shader(const std::string& filename, const VertexAttributes& attributes, const Uniforms& uniforms) :
    m_program{ 0 }, m_uniforms{}
{
    createAndAttach(filename);

    GLuint index = 0;
    for (const auto& attribute : attributes)
    {
        glBindAttribLocation(m_program, index, attribute.name.c_str());
    }

    compile();
    saveUniformLocations(uniforms);
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

void ZFX::Shader::createAndAttach(const std::string& filename)
{
    m_program = glCreateProgram();

    m_shaders[VERTEX_SHADER] = create(load(filename + ".vs"), GL_VERTEX_SHADER);
    m_shaders[FRAGMENT_SHADER] = create(load(filename + ".fs"), GL_FRAGMENT_SHADER);

    for (unsigned int i = 0; i < NUM_SHADERS; ++i)
    {
        glAttachShader(m_program, m_shaders[i]);
    }
}

void ZFX::Shader::compile()
{
    glLinkProgram(m_program);
    checkError(m_program, GL_LINK_STATUS, true, "glLinkProgram failed: ");

    glValidateProgram(m_program);
    checkError(m_program, GL_VALIDATE_STATUS, true, "glValidateProgram failed: ");
}

void ZFX::Shader::saveUniformLocations(const Uniforms& uniforms)
{
    for (const auto& u : uniforms)
    {
        saveSingleUniform(u);
    }
}

bool ZFX::Shader::saveSingleUniform(const std::string name)
{
    GLint location = glGetUniformLocation(m_program, name.c_str());
    if (UNIFORM_NOT_FOUND != location)
    {
        m_uniforms.insert( { name, location } );
        return true;
    }
    return false;
}

void ZFX::Shader::bind()
{
    glUseProgram(m_program);
}

void ZFX::Shader::update(const Transform& transform, const Camera& camera)
{
    GLint location = uniformLocation(TRANSFORM_UNIFORM);
    if (UNIFORM_NOT_FOUND != location)
    {
        glm::mat4 model = camera.getViewProjection() * transform.getModel();
        glUniformMatrix4fv(location, 1, GL_FALSE, &model[0][0]);
    }
}

void ZFX::Shader::update(const glm::mat4& transform)
{
    GLint location = uniformLocation(TRANSFORM_UNIFORM);
    if (UNIFORM_NOT_FOUND != location)
    {
        glUniformMatrix4fv(location, 1, GL_FALSE, &transform[0][0]);
    }
}

GLint ZFX::Shader::uniformLocation(const std::string& uniform) const
{
    if (m_uniforms.find(uniform) != m_uniforms.end())
    {
        return m_uniforms.at(uniform);
    }

    return UNIFORM_NOT_FOUND;
}

void ZFX::Shader::checkError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMsg)
{
    GLint success = 0;
    GLchar error[1024] = { 0 };

    if (isProgram)
    {
        glGetProgramiv(shader, flag, &success);
    }
    else
    {
        glGetShaderiv(shader, flag, &success);
    }

    if (success == GL_FALSE)
    {
        if (isProgram)
        {
            glGetProgramInfoLog(shader, sizeof(error), NULL, error);
        }
        else
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
    }
    else
    {
        throw std::runtime_error{ "Unable to load shader: " + fileName };
    }

    return output;
}

GLuint ZFX::Shader::create(const std::string& text, GLenum shaderType)
{
    GLuint shader = glCreateShader(shaderType);

    if (0 == shader)
    {
        throw std::runtime_error{ "glCreateShader failed, shader type " + shaderType };
    }
    else
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
