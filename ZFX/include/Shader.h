#pragma once
#include "Vertex.h"
#include "zfxdefs.h"
#include <glm/glm.hpp>


namespace ZFX
{
    class Camera;
    class Transform;

    using Uniforms = std::vector<std::string>;

    class Shader
    {
        /* Transform uniform is always the first uniform */
        static constexpr uint32_t U_TRANSFORM = 0;

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

    protected:
        enum
        {
            VERTEX_SHADER,
            FRAGMENT_SHADER,

            NUM_SHADERS
        };

        enum class ShaderType
        {
            Freetype
        };

        Shader(const std::string& filename, ShaderType type);
        void createAndAttach(const std::string& filename);
        void compile();
        void setUniforms(const Uniforms& uniforms);
        bool setSingleUniform(const std::string name);

        static void checkError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMsg);
        static std::string load(const std::string& fileName);
        static GLuint create(const std::string& text, GLenum shaderType);

    protected:
        bool m_hasTransform;
        GLuint m_program;
        GLuint m_shaders[NUM_SHADERS];
        std::vector<GLint> m_uniforms;
    };
}

