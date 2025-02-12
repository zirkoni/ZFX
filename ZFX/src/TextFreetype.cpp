#include "TextFreetype.h"
#include "Window.h"
#include <glm/gtc/matrix_transform.hpp>


const std::string VERTEX_SHADER =
R"(
#version 330 core
layout (location = 0) in vec2 v_in_vertex; // vec2 position

out V_OUT
{
    vec2 texCoord;
    flat int index;
} v_out;

uniform mat4 u_transforms[)" + std::to_string(ZFX::TextFreetype::U_ARRAY_LIMIT) + R"(];
uniform mat4 u_viewProjection;

void main()
{
    gl_Position = u_viewProjection * u_transforms[gl_InstanceID] * vec4(v_in_vertex.xy, 0.0, 1.0);
    v_out.index = gl_InstanceID;
    v_out.texCoord.x = v_in_vertex.x;
   v_out.texCoord.y = 1.0f - v_in_vertex.y;
}
)";

const std::string FRAGMENT_SHADER =
R"(
#version 330 core
in V_OUT
{
    vec2 texCoord;
    flat int index;
} f_in;

out vec4 f_out_colour;
uniform sampler2DArray u_diffuse;
uniform int u_charMap[)" + std::to_string(ZFX::TextFreetype::U_ARRAY_LIMIT) + R"(];
uniform vec4 u_textColour;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_diffuse, vec3(f_in.texCoord.xy, u_charMap[f_in.index])).r);
    f_out_colour = u_textColour * sampled;
}
)";


ZFX::TextFreetype::TextFreetype(const std::string& font, uint32_t winWidth, uint32_t winHeight) :
    m_vao{ 0 }, m_vbo{ 0 }, m_ySizeMax{ 0 }, m_shader{ {VERTEX_SHADER, FRAGMENT_SHADER, "", false} }
{
    handleWindowResize(winWidth, winHeight);

    glGenTextures(1, &m_textureArray);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_textureArray);
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_R8, TEXTURE_SIZE, TEXTURE_SIZE, 128, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

    GLint al;
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &al);
    init(font);

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    constexpr GLfloat vertexData[] =
    {
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 0.0f
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glPixelStorei(GL_UNPACK_ALIGNMENT, al);

    for(int i = 0; i < U_ARRAY_LIMIT; ++i)
    {
        m_transforms.push_back(glm::mat4{1.0f});
        m_charMap.push_back(0);
    }
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
        throw ZFX::Exception{ __FILE__, __LINE__, "FT_Init_FreeType failed" };
    }

    FT_Face face;
    if (FT_New_Face(ftLib, font.c_str(), 0, &face))
    {
        throw ZFX::Exception{ __FILE__, __LINE__, "FT_New_Face failed, font: " + font };
    }

    FT_Set_Pixel_Sizes(face, TEXTURE_SIZE, TEXTURE_SIZE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    loadCharacters(face);

    FT_Done_Face(face);
    FT_Done_FreeType(ftLib);
}

void ZFX::TextFreetype::loadCharacters(const FT_Face& face)
{
    for (char c = FIRST_ASCII_CODE; c <= LAST_ASCII_CODE; ++c)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cerr << "FT_Load_Char failed to load glyph " << c << std::endl;
            continue;
        }

        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, int(c),
                        face->glyph->bitmap.width, face->glyph->bitmap.rows, 1, GL_RED,
                        GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

        // To prevent certain artifacts when a character is not rendered exactly on pixel boundaries,
        // we should clamp the texture at the edges, and enable linear interpolation
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Store character for later use
        Character character =
        {
            int(c),
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<GLuint>(face->glyph->advance.x)
        };

        if(m_ySizeMax < character.size.y)
        {
            m_ySizeMax = character.size.y;
        }

        m_characters.insert(std::pair<char, Character>(c, character));
    }

    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

void ZFX::TextFreetype::handleWindowResize(uint32_t newWidth, uint32_t newHeight)
{
    // Set aspect ratio so that text is not distorted
    glm::mat4 viewProjection = glm::ortho(0.0f, static_cast<float>(newWidth), 0.0f, static_cast<float>(newHeight));

    m_shader.bind();
    m_shader.setUniform("u_viewProjection", viewProjection);
}

void ZFX::TextFreetype::drawText(std::string_view text, float x, float y, float scale,
        const glm::vec4& colour, float lineSpacing)
{
    m_shader.bind();
    m_shader.setUniform("u_textColour", colour);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_textureArray);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    const float copyX = x;
    int index = 0;
    for (const auto& c : text)
    {
        // New line is not in m_characters so handle it separately
        if(c == '\n')
        {
            y -= m_ySizeMax * lineSpacing * scale;
            x = copyX;
            continue;
        }

        auto chIter = m_characters.find(c);
        if(chIter == m_characters.end())
        {
            std::cerr << "Non-rendered character detected" << std::endl;
            continue;
        }

        if(c == ' ')
        {
            x += (chIter->second.advance >> 6) * scale;
        } else
        {
            float xpos = x + chIter->second.bearing.x * scale;
            float ypos = y - (TEXTURE_SIZE - chIter->second.bearing.y) * scale;
            float w = TEXTURE_SIZE * scale;
            float h = TEXTURE_SIZE * scale;

            m_transforms.at(index) = glm::translate(glm::mat4{1.0f}, glm::vec3{xpos, ypos, 0.0f})
                            * glm::scale(glm::mat4{1.0f}, glm::vec3{w, h, 0.0f});

            m_charMap.at(index) = chIter->second.textureID;

            x += (chIter->second.advance >> 6) * scale;
            ++index;

            if(index == U_ARRAY_LIMIT)
            {
                renderCall(index);
                index = 0;
            }
        }
    }

    renderCall(index);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

void ZFX::TextFreetype::renderCall(int length)
{
    if(length > 0)
    {
        m_shader.setUniformArray("u_transforms", m_transforms, length);
        m_shader.setUniformArray("u_charMap", m_charMap, length);
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, length);
    }
}

