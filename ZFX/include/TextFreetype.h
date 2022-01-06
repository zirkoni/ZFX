#pragma once
#include "Shader.h"
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H


namespace ZFX
{
    extern const std::string TEXTCOLOUR_UNIFORM;

    class TextFreetype
    {
        struct Character
        {
            GLuint textureID; // ID handle of the glyph texture
            glm::ivec2   size;      // Size of glyph
            glm::ivec2   bearing;   // Offset from baseline to left/top of glyph
            GLuint advance;   // Horizontal offset to advance to next glyph
        };

        class TextShader : public Shader
        {
        public:
            TextShader(const std::string& filename) :
                Shader{ filename, { TRANSFORM_UNIFORM, TEXTCOLOUR_UNIFORM } } {}
        };

    public:
        TextFreetype(const std::string& font, const std::string& shader);
        ~TextFreetype();

        void drawText(const std::string& text, float x, float y, float scale, glm::vec3 colour);

    private:
        void init(const std::string& font);
        void loadCharacters(const FT_Face& face, uint32_t numCharacters);

    private:
        GLuint m_vao;
        GLuint m_vbo;

        TextShader m_shader;
        std::map<char, Character> m_characters;
    };
}


