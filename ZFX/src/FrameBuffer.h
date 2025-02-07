#include <GL/glew.h>
#include <glm/glm.hpp>


namespace ZFX
{
    class FrameBuffer
    {
    public:
        /* Create MSAA framebuffer if numMSAASamples > 0
         * Note that an MSAA framebuffer cannot be used directly for post-processing
         * so you'd need a 2nd intermediate non-MSAA framebuffer for that.
         */
        FrameBuffer(int numMSAASamples = 0, uint32_t bitsToClear = GL_COLOR_BUFFER_BIT);
        ~FrameBuffer();

        void reset(int numMSAASamples);

        void bind();
        static void bindDefault();

        // TODO: can be only 1 (or 2) type
        void attachTextureBuffer(GLsizei width, GLsizei height);
        void attachRenderBuffer(GLsizei width, GLsizei height);
        void attachDepthBuffer(GLsizei width, GLsizei height);
        void attachDepthCubeMap(GLsizei width, GLsizei height);

        void bindTextureBuffer();
        void bindDepthBuffer();
        void bindDepthCubeMap();

        void clear(); // clear to black
        void clear(float r, float g, float b, float a); // clear to user defined colour
        void clear(glm::vec4 bgColour); // clear to user defined colour

        GLuint fbo() const { return m_fbo; }

    private:
        void deleteResources();

        GLuint m_fbo;
        // TODO: can be only 1 type
        GLuint m_textureBuffer;
        GLuint m_rbo;
        GLuint m_depthBuffer;

        bool m_hasTextureBuffer = false;
        bool m_hasRbo = false;
        bool m_hasDepthBuffer = false;

        int m_numMSAASamples;
        uint32_t m_bitsToClear;
    };
}
