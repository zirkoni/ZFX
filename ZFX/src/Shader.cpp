#include "Shader.h"
#include "Camera.h"
#include "Transform.h"
#include <fstream>


ZFX::Shader::Shader(const ShaderSource &source, bool validate)
{
    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint geometryShader;

    m_program = glCreateProgram();

    vertexShader = create(source.areFiles, source.vertex, GL_VERTEX_SHADER);
    fragmentShader = create(source.areFiles, source.fragment, GL_FRAGMENT_SHADER);

    bool hasGeomShader = source.geometry.length() > 0;
    if(hasGeomShader)
    {
        geometryShader = create(source.areFiles, source.geometry, GL_GEOMETRY_SHADER);
    }

    glAttachShader(m_program, vertexShader);
    glAttachShader(m_program, fragmentShader);

    if(hasGeomShader)
    {
        glAttachShader(m_program, geometryShader);
    }

    compile(validate);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    if(hasGeomShader)
    {
        glDeleteShader(geometryShader);
    }

    saveUniformLocations();
}

ZFX::Shader::~Shader()
{
    glDeleteProgram(m_program);
}

void ZFX::Shader::compile(bool validate)
{
    glLinkProgram(m_program);
    checkError(m_program, GL_LINK_STATUS, true, "glLinkProgram failed");

    if(validate)
    {
        this->validate();
    }
}

void ZFX::Shader::validate()
{
    glValidateProgram(m_program);
    checkError(m_program, GL_VALIDATE_STATUS, true, "glValidateProgram failed");
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
        throw ZFX::Exception{ __FILE__, __LINE__, "glGetActiveUniform returned uniform name with length 0" };
    }

    GLint uniformLocation = glGetUniformLocation(m_program, buffer);
    std::string uniformName{ buffer };
    const bool isArrayUniform = uniformName.find('[') != std::string::npos && uniformName.find(']') != std::string::npos;

    if(uniformLocation == UNIFORM_NOT_FOUND)
    {
        throw ZFX::Exception{ __FILE__, __LINE__, "glGetUniformLocation failed to get uniform: " + uniformName };
    }

    m_uniforms.insert( { uniformName, uniformLocation } );

    // Only index 0 is found automatically
    if(isArrayUniform)
    {
        size_t idx = 1;
        while(true)
        {
            uniformName = uniformName.substr(0, uniformName.find('[')) + "[" + std::to_string(idx) + "]";
            uniformLocation = glGetUniformLocation(m_program, uniformName.c_str());

            if(uniformLocation != UNIFORM_NOT_FOUND)
            {
                m_uniforms.insert( { uniformName, uniformLocation } );
                ++idx;
            } else
            {
                break;
            }
        }
    }
}

void ZFX::Shader::bind()
{
    glUseProgram(m_program);
}

void ZFX::Shader::update(const Transform& transform, const Camera& camera)
{
    update(transform.getModel());
    update(camera, "u_viewProjection");
}

void ZFX::Shader::update(const glm::mat4& transform)
{
    GLint modelLocation = uniformLocation("u_model");
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
        throw ZFX::Exception{ __FILE__, __LINE__, "No such uniform: " + uniform };
    }

    return iter->second;
}

void ZFX::Shader::setUniform(const std::string& uniform, int value)
{
    GLint loc = uniformLocation(uniform);
    glUniform1i(loc, value);
}

void ZFX::Shader::setUniform(const std::string& uniform, float value)
{
    GLint loc = uniformLocation(uniform);
    glUniform1f(loc, value);
}

void ZFX::Shader::setUniform(const std::string& uniform, const glm::vec2& value)
{
    GLint loc = uniformLocation(uniform);
    glUniform2f(loc, value.x, value.y);
}

void ZFX::Shader::setUniform(const std::string& uniform, const glm::vec3& value)
{
    GLint loc = uniformLocation(uniform);
    glUniform3f(loc, value.x, value.y, value.z);
}

void ZFX::Shader::setUniform(const std::string& uniform, const glm::vec4& value)
{
    GLint loc = uniformLocation(uniform);
    glUniform4f(loc, value.x, value.y, value.z, value.w);
}

void ZFX::Shader::setUniform(const std::string& uniform, const glm::mat4& value)
{
    GLint loc = uniformLocation(uniform);
    glUniformMatrix4fv(loc, 1, GL_FALSE, &value[0][0]);
}

void ZFX::Shader::setUniformArray(const std::string& uniform, const std::vector<int>& values, int numValues)
{
    GLint loc = glGetUniformLocation(m_program, uniform.c_str());

    if(loc == UNIFORM_NOT_FOUND)
    {
        throw ZFX::Exception{ __FILE__, __LINE__, "glGetUniformLocation failed to get uniform: " + uniform };
    }

    glUniform1iv(loc, numValues, &values[0]);
}

void ZFX::Shader::setUniformArray(const std::string& uniform, const std::vector<glm::mat4>& values, int numValues)
{
    GLint loc = glGetUniformLocation(m_program, uniform.c_str());

    if(loc == UNIFORM_NOT_FOUND)
    {
        throw ZFX::Exception{ __FILE__, __LINE__, "glGetUniformLocation failed to get uniform: " + uniform };
    }

    glUniformMatrix4fv(loc, numValues, GL_FALSE, &values[0][0][0]);
}

void ZFX::Shader::checkError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMsg,
        const std::string& errorMsg2)
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

        throw ZFX::Exception{ __FILE__, __LINE__, errorMsg + ":\n" + errorMsg2 + "\n" + error };
    }
}

void ZFX::Shader::loadFromFile(const std::string& fileName, GLuint shader)
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
        throw ZFX::Exception{ __FILE__, __LINE__, "Unable to load shader: " + fileName };
    }

    loadFromString(output, shader);
}

void ZFX::Shader::loadFromString(const std::string& source, GLuint shader)
{
    const GLchar* p[1];
    p[0] = source.c_str();
    GLint lengths[1];
    lengths[0] = (GLint)source.length();

    glShaderSource(shader, 1, p, lengths);
    glCompileShader(shader);
}

GLuint ZFX::Shader::create(bool isFile, const std::string& source, GLenum shaderType)
{
    GLuint shader = glCreateShader(shaderType);

    if (0 == shader)
    {
        throw ZFX::Exception{ __FILE__, __LINE__, "glCreateShader failed, shader type " + shaderType };
    }
    else
    {
        if(isFile)
        {
            loadFromFile(source, shader);
        } else
        {
            loadFromString(source, shader);
        }

        checkError(shader, GL_COMPILE_STATUS, false, std::string{"Error compiling shader"}, source);
    }

    return shader;
}
