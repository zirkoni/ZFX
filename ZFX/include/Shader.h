#pragma once
#include "Vertex.h"
#include "zfxdefs.h"
#include <glm/glm.hpp>
#include <unordered_map>


namespace ZFX
{
    class Camera;
    class Transform;

    class Shader
    {
        using UniformMap = std::unordered_map<std::string, const GLint>;

    public:
        Shader(const std::string& filename);
        Shader(const Shader& other) = delete;
        Shader& operator=(const Shader& other) = delete;
        virtual ~Shader();

        /* Use this shader */
        void bind();

        /* Apply transformations */
        void update(const Transform& transform, const Camera& camera);
        void update(const glm::mat4& transform);
        void update(const Camera& camera, const std::string& uniform);

        GLuint programId() const { return m_program; }
        GLint uniformLocation(const std::string& uniform) const;

        void setUniformInt(const std::string& uniform, int value);
        void setUniformFloat(const std::string& uniform, float value);
        void setUniformVec2(const std::string& uniform, const glm::vec2& value);
        void setUniformVec3(const std::string& uniform, const glm::vec3& value);
        void setUniformVec4(const std::string& uniform, const glm::vec4& value);

        void setUniformMat4(const std::string& uniform, const glm::mat4& value);

    protected:
        void createAndAttach(const std::string& filename, GLuint& vertexShader, GLuint& fragmentShader);
        void compile();
        void saveUniformLocations();
        void saveSingleUniform(const GLint BUF_SIZE, GLuint idx);

        static void checkError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMsg);
        static std::string load(const std::string& fileName);
        static GLuint create(const std::string& fileName, GLenum shaderType);

    protected:
        GLuint m_program;
        UniformMap m_uniforms;
    };
}

