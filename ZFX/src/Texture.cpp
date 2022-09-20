#include "Texture.h"
#include "zfxdefs.h"
#include <SDL2/SDL_image.h>
#include <cassert>

#ifndef USE_SDL2_IMAGE
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

ZFX::Texture::Texture(const std::string& filename) : m_texture{ 0 }
{
#ifdef USE_SDL2_IMAGE
    SDL_Surface* surface;
    surface = IMG_Load(filename.c_str());

    if (surface == nullptr)
    {
        std::string msg = "IMG_Load failed: ";
        msg += std::string{ IMG_GetError() };
        throw ZFX::Exception{ msg };
    }

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    // Define what happens when reading outside texture width/height (or x/y)
    // GL_REPEAT: repeat, i.e. go back to beginning
    // GL_CLAMP: out of bounds -> default (black?) pixels
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Define what happens when rendered object size is not equal to texture size (interpolating)
    // GL_LINEAR: linear interpolation
    // GL_NEAREST: ...
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // smaller
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // bigger

    if (surface->format->Amask != 0)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
    }
    
    SDL_FreeSurface(surface);
#else
    int width;
    int height;
    int numComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* imageData = stbi_load(filename.c_str(), &width, &height, &numComponents, 4);

    if (imageData == nullptr)
    {
        std::string msg = "Texture loading failed: " + filename;
        throw ZFX::Exception{ msg };
    }
    else
    {
#if 1
        glGenTextures(1, &m_texture);
        glBindTexture(GL_TEXTURE_2D, m_texture);

        // Define what happens when reading outside texture width/height (or x/y)
        // GL_REPEAT: repeat, i.e. go back to beginning
        // GL_CLAMP: out of bounds -> default (black?) pixels
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Define what happens when rendered object size is not equal to texture size (interpolating)
        // GL_LINEAR: linear interpolation
        // GL_NEAREST: ...
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // smaller
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // bigger

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D);
#else // TODO: is there any benefit for checking the format?
        GLenum format;
        if      (numComponents == 1) format = GL_RED;
        else if (numComponents == 3) format = GL_RGB; // Demo1 texture has no alpha but alpha is used => glitch
        else if (numComponents == 4) format = GL_RGBA;

        glGenTextures(1, &m_texture);
        glBindTexture(GL_TEXTURE_2D, m_texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D);
#endif

        stbi_image_free(imageData);
    }
#endif
}

ZFX::Texture::~Texture()
{
    glDeleteTextures(1, &m_texture);
}

void ZFX::Texture::bind(uint32_t unit)
{
    assert(unit >= 0 && unit <= 31);

    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, m_texture);
}
