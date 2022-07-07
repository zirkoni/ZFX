#include "TextFreetype.h"
#include "Window.h"
#include <glm/gtc/matrix_transform.hpp>
#include <stdexcept>
#include <iostream>


const std::string VERTEX_SHADER =
    "#version 330 core\n"
    "layout (location = 0) in vec4 vertex; // xy = vec2 position, zw= vec2 texture coordinates\n"
    "out vec2 texCoord;"
    "uniform mat4 model;"
    "void main() {"
    "    gl_Position = model * vec4(vertex.xy, 0.0, 1.0);"
    "    texCoord = vertex.zw;"
    "}";

const std::string FRAGMENT_SHADER =
    "#version 330 core\n"
    "in vec2 texCoord;"
    "out vec4 fragColourOut;"
    "uniform sampler2D diffuse;"
    "uniform vec4 textColour;"
    "void main() {"
    "    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(diffuse, texCoord).r);"
    "    fragColourOut = textColour * sampled;"
    "}";

ZFX::TextFreetype::TextFreetype(const std::string& font) :
    m_vao{ 0 }, m_vbo{ 0 }, m_shader{ VERTEX_SHADER, FRAGMENT_SHADER, false }
{
    glm::mat4 transform = glm::ortho(0.0f, static_cast<float>(Window::width()), 0.0f,
            static_cast<float>(Window::height()));

    m_shader.bind();
    m_shader.update(transform);

    init(font);

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    // We need 6 verteces/character (2 triangles)
    // For each vertex we have 2D position and texture coordinates = 4 floats
    glBufferData(GL_ARRAY_BUFFER, 6 * 4 * sizeof(float), NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

ZFX::TextFreetype::~TextFreetype()
{
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
}

void ZFX::TextFreetype::init(const std::string& font)
{
    FT_Library ftLib;
    if (FT_Init_FreeType(&ftLib))
    {
        throw std::runtime_error{ "FT_Init_FreeType failed" };
    }

    FT_Face face;
    if (FT_New_Face(ftLib, font.c_str(), 0, &face))
    {
        throw std::runtime_error{ "FT_New_Face failed, font: " + font };
    }

    FT_Set_Pixel_Sizes(face, 0, 48);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    loadCharacters(face, 128);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    FT_Done_Face(face);
    FT_Done_FreeType(ftLib);
}

void ZFX::TextFreetype::loadCharacters(const FT_Face& face, uint32_t numCharacters)
{
    for (char c = 0; c < numCharacters; ++c)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cerr << "FT_Load_Char failed to load glyph " << c << std::endl;
            continue;
        }

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0,
            GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

        // To prevent certain artifacts when a character is not rendered exactly on pixel boundaries,
        // we should clamp the texture at the edges, and enable linear interpolation
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Store character for later use
        Character character =
        {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<GLuint>(face->glyph->advance.x)
        };

        m_characters.insert(std::pair<char, Character>(c, character));
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

void ZFX::TextFreetype::drawText(const std::string& text, float x, float y, float scale,
        const glm::vec4& colour)
{
    m_shader.bind();
    m_shader.setUniformVec4("textColour", colour);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(m_vao);

    for (const auto c : text)
    {
        const Character& ch = m_characters[c];

        float xpos = x + ch.bearing.x * scale;
        float ypos = y - (ch.size.y - ch.bearing.y) * scale;

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        // update VBO for each character, CCW
#if 1
        float vertices[6][4] =
        {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
#else
        // CW
        float vertices[6][4] =
        {
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f }
        };
#endif

        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.textureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
