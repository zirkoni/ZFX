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

        // 2 files without extension: baseFileName.vs & baseFileName.fs
        Shader(const std::string& baseFileName);

        // 2 files with extension, filenames can be whatever
        Shader(const std::string& vertexFileName, const std::string& fragFileName);

        // Pass the source directly (no files). Dummy argument unused
        Shader(const std::string& vertexSource, const std::string& fragSource, bool dummy);

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
        void compile();
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

