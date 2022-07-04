#include "Light.h"
#include "Shader.h"


ZFX::Light::Light(const std::string& name, Shader &shader): m_name{ name }, m_shader{ shader }
{
}

void ZFX::Light::bind()
{
    m_shader.bind();
}

void ZFX::Light::setAmbient(const glm::vec3& amb)
{
    m_shader.setUniformVec3(m_name + ".ambient", amb);
}

void ZFX::Light::setDiffuse(const glm::vec3& dif)
{
    m_shader.setUniformVec3(m_name + ".diffuse", dif);
}

void ZFX::Light::setSpecular(const glm::vec3& spec)
{
    m_shader.setUniformVec3(m_name + ".specular", spec);
}

///////////////////////
ZFX::DirectionalLight::DirectionalLight(const std::string& name, Shader &shader): Light{ name, shader }
{
}

void ZFX::DirectionalLight::setDirection(const glm::vec3 &dir)
{
    m_shader.setUniformVec3(m_name + ".direction", dir);
}

///////////////////////
ZFX::PointLight::PointLight(const std::string& name, Shader &shader): Light{ name, shader }
{
}

void ZFX::PointLight::setPosition(const glm::vec3 &pos)
{
    m_shader.setUniformVec3(m_name + ".position", pos);
}

void ZFX::PointLight::setConstant(float c)
{
    m_shader.setUniformFloat(m_name + ".constant", c);
}

void ZFX::PointLight::setLinear(float l)
{
    m_shader.setUniformFloat(m_name + ".linear", l);
}

void ZFX::PointLight::setQuadratic(float q)
{
    m_shader.setUniformFloat(m_name + ".quadratic", q);
}

///////////////////////
ZFX::SpotLight::SpotLight(const std::string& name, Shader &shader): PointLight{ name, shader }
{
}

void ZFX::SpotLight::setDirection(const glm::vec3 &dir)
{
    m_shader.setUniformVec3(m_name + ".direction", dir);
}

void ZFX::SpotLight::setCutOff(float c)
{
    m_shader.setUniformFloat(m_name + ".cutOff", c);
}

void ZFX::SpotLight::setOuterCutOff(float o)
{
    m_shader.setUniformFloat(m_name + ".outerCutOff", o);
}
