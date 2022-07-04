#include "Shader.h"
#include "Camera.h"
#include "Transform.h"
#include <stdexcept>
#include <fstream>


ZFX::Shader::Shader(const std::string& filename)
{
    GLuint vertexShader;
    GLuint fragmentShader;

    createAndAttach(filename, vertexShader, fragmentShader);

    compile();

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    saveUniformLocations();
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

void ZFX::Shader::saveUniformLocations()
{
    GLint uniformMaxLength = 0;
    GLint numUniforms = 0;

    glGetProgramiv(m_program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniformMaxLength);
    glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &numUniforms);

    for(GLuint i = 0; i < numUniforms; ++i)
    {
        saveSingleUniform(uniformMaxLength, i);
    }
}

void ZFX::Shader::saveSingleUniform(const GLint BUF_SIZE, GLuint idx)
{
    char buffer[BUF_SIZE];
    GLsizei uLength = 0;
    GLint uSize = 0;
    GLenum uType = 0;

    glGetActiveUniform(m_program, idx, (GLsizei)sizeof(buffer), &uLength, &uSize, &uType, buffer);

    if(uLength == 0)
    {
        throw std::runtime_error{ "glGetActiveUniform returned uniform name with length 0" };
    }

    const GLint uniformLocation = glGetUniformLocation(m_program, buffer);
    std::string uniformName{ buffer };

    if(uniformLocation == UNIFORM_NOT_FOUND)
    {
        throw std::runtime_error{ "glGetUniformLocation failed to get uniform: " + uniformName };
    }

    m_uniforms.insert( { uniformName, uniformLocation } );
}

void ZFX::Shader::bind()
{
    glUseProgram(m_program);
}

void ZFX::Shader::update(const Transform& transform, const Camera& camera)
{
    update(transform.getModel());
    update(camera, "viewProjection");
}

void ZFX::Shader::update(const glm::mat4& transform)
{
    GLint modelLocation = uniformLocation("model");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &transform[0][0]);
}

void ZFX::Shader::update(const Camera& camera, const std::string& uniform)
{
    GLint vpLocation = uniformLocation(uniform);
    glUniformMatrix4fv(vpLocation, 1, GL_FALSE, &camera.getViewProjection()[0][0]);
}

GLint ZFX::Shader::uniformLocation(const std::string& uniform) const
{
    auto iter = m_uniforms.find(uniform);
    if(iter == m_uniforms.end())
    {
        throw std::runtime_error{ "No such uniform: " + uniform };
    }

    return iter->second;
}

void ZFX::Shader::setUniformInt(const std::string& uniform, int value)
{
    bind();
    GLint loc = uniformLocation(uniform);
    glUniform1i(loc, value);
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

void ZFX::Shader::setUniformMat4(const std::string& uniform, const glm::mat4& value)
{
    bind();
    GLint loc = uniformLocation(uniform);
    glUniformMatrix4fv(loc, 1, GL_FALSE, &value[0][0]);
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
