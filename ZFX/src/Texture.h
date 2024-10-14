#pragma once
#include <string>
#include <GL/glew.h>


namespace ZFX
{
    class Texture
    {
    public:
        Texture(const std::string& filename);
        Texture(const Texture& other) = delete;
        Texture& operator=(const Texture& other) = delete;
        ~Texture();

        void bind(uint32_t unit);

    private:
        GLuint m_texture;
    };
}
