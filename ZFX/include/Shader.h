#pragma once
#include "Vertex.h"
#include "zfxdefs.h"
#include <glm/glm.hpp>
#include <unordered_map>


namespace ZFX
{
    class Camera;
    class Transform;

    using Uniforms = std::vector<std::string>;

    extern const std::string MODEL_UNIFORM;
    extern const std::string VIEW_PROJECTION_UNIFORM;
    extern const Uniforms DEFAULT_UNIFORMS;

    class Shader
    {
        using UniformMap = std::unordered_map<std::string, GLint>;

    public:
        Shader(const std::string& filename);
        Shader(const std::string& filename, const Uniforms& uniforms);
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

        void setUniformFloat(const std::string& uniform, float value);
        void setUniformVec2(const std::string& uniform, const glm::vec2& value);
        void setUniformVec3(const std::string& uniform, const glm::vec3& value);
        void setUniformVec4(const std::string& uniform, const glm::vec4& value);

    protected:
        void createAndAttach(const std::string& filename, GLuint& vertexShader, GLuint& fragmentShader);
        void compile();
        void saveUniformLocations(const Uniforms& uniforms);
        void saveSingleUniform(const std::string& name);

        static void checkError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMsg);
        static std::string load(const std::string& fileName);
        static GLuint create(const std::string& fileName, GLenum shaderType);

    protected:
        GLuint m_program;
        UniformMap m_uniforms;
    };
}

