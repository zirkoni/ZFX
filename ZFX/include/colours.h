#pragma once
#include <glm/glm.hpp>

namespace ZFX
{
// Only 0 or 1
static const glm::vec4 BLACK    { 0.0f, 0.0f, 0.0f, 1.0f };
static const glm::vec4 WHITE    { 1.0f, 1.0f, 1.0f, 1.0f };
static const glm::vec4 RED      { 1.0f, 0.0f, 0.0f, 1.0f };
static const glm::vec4 GREEN    { 0.0f, 1.0f, 0.0f, 1.0f };
static const glm::vec4 BLUE     { 0.0f, 0.0f, 1.0f, 1.0f };
static const glm::vec4 YELLOW   { 1.0f, 1.0f, 0.0f, 1.0f };
static const glm::vec4 CYAN     { 0.0f, 1.0f, 1.0f, 1.0f };
static const glm::vec4 MAGENTA  { 1.0f, 0.0f, 1.0f, 1.0f };

// Grays
glm::vec4 makeGray(float lightAmount)
{
    assert(lightAmount > 0.0f && lightAmount < 1.0f);

    glm::vec4 colour = lightAmount * WHITE;
    colour.a = 1.0f;
    return colour;
}

}

