#include "Shader.h"
#include "Camera.h"
#include "Transform.h"
#include <stdexcept>
#include <fstream>

const std::string ZFX::MODEL_UNIFORM = "model";
const std::string ZFX::VIEW_PROJECTION_UNIFORM = "viewProjection";
const ZFX::Uniforms ZFX::DEFAULT_UNIFORMS = { ZFX::MODEL_UNIFORM, ZFX::VIEW_PROJECTION_UNIFORM };

ZFX::Shader::Shader(const std::string& filename): Shader{ filename, DEFAULT_UNIFORMS }
{
}

ZFX::Shader::Shader(const std::string& filename, const Uniforms& uniforms) :
    m_program{ 0 }, m_uniforms{}
{
    GLuint vertexShader;
    GLuint fragmentShader;

    createAndAttach(filename, vertexShader, fragmentShader);

    compile();

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    saveUniformLocations(uniforms);
}

ZFX::Shader::~Shader()
{
    glDeleteProgram(m_program);
}

void ZFX::Shader::createAndAttach(const std::string& filename, GLuint& vertexShader, GLuint& fragmentShader)
{
    m_program = glCreateProgram();

    vertexShader = create(filename + ".vs", GL_VERTEX_SHADER);
    fragmentShader = create(filename + ".fs", GL_FRAGMENT_SHADER);

    glAttachShader(m_program, vertexShader);
    glAttachShader(m_program, fragmentShader);
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

void ZFX::Shader::saveSingleUniform(const std::string& name)
{
    GLint location = glGetUniformLocation(m_program, name.c_str());
    if (UNIFORM_NOT_FOUND != location)
    {
        m_uniforms.insert( { name, location } );
    }
    else
    {
        throw std::runtime_error{ "glGetUniformLocation failed: " + name };
    }
}

void ZFX::Shader::bind()
{
    glUseProgram(m_program);
}

void ZFX::Shader::update(const Transform& transform, const Camera& camera)
{
    update(transform.getModel());
    update(camera, VIEW_PROJECTION_UNIFORM);
}

void ZFX::Shader::update(const glm::mat4& transform)
{
    GLint modelLocation = uniformLocation(MODEL_UNIFORM);
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &transform[0][0]);
}

void ZFX::Shader::update(const Camera& camera, const std::string& uniform)
{
    GLint vpLocation = uniformLocation(VIEW_PROJECTION_UNIFORM);
    glUniformMatrix4fv(vpLocation, 1, GL_FALSE, &camera.getViewProjection()[0][0]);
}

GLint ZFX::Shader::uniformLocation(const std::string& uniform) const
{
    return m_uniforms.at(uniform);
}

void ZFX::Shader::setUniformFloat(const std::string& uniform, float value)
{
    bind();
    GLint loc = uniformLocation(uniform);
    glUniform1f(loc, value);
}

void ZFX::Shader::setUniformVec2(const std::string& uniform, const glm::vec2& value)
{
    bind();
    GLint loc = uniformLocation(uniform);
    glUniform2f(loc, value.x, value.y);
}

void ZFX::Shader::setUniformVec3(const std::string& uniform, const glm::vec3& value)
{
    bind();
    GLint loc = uniformLocation(uniform);
    glUniform3f(loc, value.x, value.y, value.z);
}

void ZFX::Shader::setUniformVec4(const std::string& uniform, const glm::vec4& value)
{
    bind();
    GLint loc = uniformLocation(uniform);
    glUniform4f(loc, value.x, value.y, value.z, value.w);
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

GLuint ZFX::Shader::create(const std::string& fileName, GLenum shaderType)
{
    GLuint shader = glCreateShader(shaderType);

    if (0 == shader)
    {
        throw std::runtime_error{ "glCreateShader failed, shader type " + shaderType };
    }
    else
    {
        const std::string text = load(fileName);
        const GLchar* p[1];
        p[0] = text.c_str();
        GLint lengths[1];
        lengths[0] = (GLint)text.length();

        glShaderSource(shader, 1, p, lengths);
        glCompileShader(shader);

        checkError(shader, GL_COMPILE_STATUS, false, std::string("Error compiling shader: " + fileName + "\n").c_str());
    }

    return shader;
}
