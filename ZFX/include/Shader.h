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

    extern const std::string TRANSFORM_UNIFORM;
    extern const Uniforms DEFAULT_UNIFORMS;

    class Shader
    {
        using UniformMap = std::unordered_map<std::string, GLint>;

    public:
        Shader(const std::string& filename, const VertexAttributes& attributes);
        Shader(const std::string& filename, const VertexAttributes& attributes, const Uniforms& uniforms);
        Shader(const Shader& other) = delete;
        Shader& operator=(const Shader& other) = delete;
        virtual ~Shader();

        /* Use this shader */
        void bind();

        /* Apply transformations */
        void update(const Transform& transform, const Camera& camera);
        void update(const glm::mat4& transform);

        GLuint programId() const { return m_program; }
        GLint uniformLocation(const std::string& uniform) const;

    protected:
        enum
        {
            VERTEX_SHADER,
            FRAGMENT_SHADER,

            NUM_SHADERS
        };

        void createAndAttach(const std::string& filename);
        void compile();
        void saveUniformLocations(const Uniforms& uniforms);
        bool saveSingleUniform(const std::string name);

        static void checkError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMsg);
        static std::string load(const std::string& fileName);
        static GLuint create(const std::string& text, GLenum shaderType);

    protected:
        GLuint m_program;
        GLuint m_shaders[NUM_SHADERS];
        UniformMap m_uniforms;
    };
}

