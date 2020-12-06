#pragma once
#include "zfxdefs.h"
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>


namespace ZFX
{
    class Camera;
    class Transform;


    class Shader
    {
    public:
        Shader(const std::string& filename, const bool useTexture = false);
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
        void compileAndSetUniforms();

        static void checkError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMsg);
        static std::string load(const std::string& fileName);
        static GLuint create(const std::string& text, GLenum shaderType);

    protected:
        bool m_useTexture;
        GLuint m_program;
        GLuint m_shaders[NUM_SHADERS];
        GLuint m_uniforms[NUM_UNIFORMS];
    };
}

