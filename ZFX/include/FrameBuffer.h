#include <GL/glew.h>
#include <glm/glm.hpp>


namespace ZFX
{
    class FrameBuffer
    {
    public:
        FrameBuffer();

        void bind();
        static void bindDefault();

        void attachTextureBuffer(GLsizei width, GLsizei height);
        void attachRenderBuffer(GLsizei width, GLsizei height);

        void bindTextureBuffer();

        void clear(); // clear to black
        void clear(float r, float g, float b, float a); // clear to user defined colour
        void clear(glm::vec4 bgColour); // clear to user defined colour

    private:
        GLuint m_fbo;
        GLuint m_textureBuffer;
        GLuint m_rbo;
    };
}
