#pragma once
#include "Shader.h"
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H


namespace ZFX
{
    class TextFreetype
    {
        struct Character
        {
            GLuint textureID; // ID handle of the glyph texture
            glm::ivec2   size;      // Size of glyph
            glm::ivec2   bearing;   // Offset from baseline to left/top of glyph
            GLuint advance;   // Horizontal offset to advance to next glyph
        };

    public:
        TextFreetype(const std::string& font);
        ~TextFreetype();

        void drawText(std::string_view text, float x, float y, float scale, const glm::vec4& colour);
        void handleWindowResize(uint32_t newWidth, uint32_t newHeight);

    private:
        void init(const std::string& font);
        void loadCharacters(const FT_Face& face, uint32_t numCharacters);

    private:
        GLuint m_vao;
        GLuint m_vbo;

        Shader m_shader;
        std::map<char, Character> m_characters;
    };
}


