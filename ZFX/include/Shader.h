#pragma once
#include "Vertex.h"
#include "zfxdefs.h"
#include <glm/glm.hpp>
#include <unordered_map>


namespace ZFX
{
    class Camera;
    class Transform;

    struct ShaderSource
    {
        const std::string& vertex;
        const std::string& fragment;
        const std::string& geometry = "";
        bool areFiles = true;
    };

    class Shader
    {
        using UniformMap = std::unordered_map<std::string, const GLint>;

    public:
        Shader(const ShaderSource& source, bool validate = true);
        Shader(const Shader& other) = delete;
        Shader& operator=(const Shader& other) = delete;
        virtual ~Shader();

        void validate();

        /* Use this shader */
        void bind();

        /* Apply transformations */
        void update(const Transform& transform, const Camera& camera);
        void update(const glm::mat4& transform);
        void update(const Camera& camera, const std::string& uniform);

        GLuint programId() const { return m_program; }
        GLint uniformLocation(const std::string& uniform) const;

        void setUniform(const std::string& uniform, int value);
        void setUniform(const std::string& uniform, float value);
        void setUniform(const std::string& uniform, const glm::vec2& value);
        void setUniform(const std::string& uniform, const glm::vec3& value);
        void setUniform(const std::string& uniform, const glm::vec4& value);
        void setUniform(const std::string& uniform, const glm::mat4& value);

    protected:
        void compile(bool validate);
        void saveUniformLocations();
        void saveSingleUniform(const GLint BUF_SIZE, GLuint idx);

        static void checkError(GLuint shader, GLuint flag, bool isProgram,
                const std::string& errorMsg, const std::string& errorMsg2 = "");
        static void loadFromFile(const std::string& fileName, GLuint shader);
        static void loadFromString(const std::string& source, GLuint shader);
        static GLuint create(bool isFile, const std::string& source, GLenum shaderType);

    protected:
        GLuint m_program;
        UniformMap m_uniforms;
    };
}

