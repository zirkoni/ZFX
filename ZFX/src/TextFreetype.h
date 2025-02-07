#pragma once
#include "Shader.h"
#include <map>
#include <vector>
#include <ft2build.h>
#include FT_FREETYPE_H


namespace ZFX
{
    class TextFreetype
    {
        struct Character
        {
            int textureID; // ID handle of the glyph texture
            glm::ivec2   size;      // Size of glyph
            glm::ivec2   bearing;   // Offset from baseline to left/top of glyph
            GLuint advance;   // Horizontal offset to advance to next glyph
        };

        static constexpr unsigned TEXTURE_SIZE = 64;
        static constexpr uint8_t FIRST_ASCII_CODE = ' ';
        static constexpr uint8_t LAST_ASCII_CODE = '~';

    public:
        static constexpr int U_ARRAY_LIMIT = 126;

        TextFreetype(const std::string& font, uint32_t winWidth, uint32_t winHeight);
        ~TextFreetype();

        void drawText(std::string_view text, float x, float y, float scale, const glm::vec4& colour, float lineSpacing = 1.0f);
        void handleWindowResize(uint32_t newWidth, uint32_t newHeight);

    private:
        void init(const std::string& font);
        void loadCharacters(const FT_Face& face);
        void renderCall(int length);

    private:
        GLuint m_vao;
        GLuint m_vbo;
        GLuint m_textureArray;
        int m_ySizeMax;

        Shader m_shader;
        std::map<char, Character> m_characters;
        std::vector<glm::mat4> m_transforms;
        std::vector<int> m_charMap;
    };
}


