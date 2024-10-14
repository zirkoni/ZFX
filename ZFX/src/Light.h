#pragma once
#include <glm/glm.hpp>
#include <string>


namespace ZFX
{
    class Shader;

    ///////////////////////
    class Light
    {
    public:
        Light(const std::string& name, Shader& shader);

        void bind();

        void setAmbient(const glm::vec3& amb);
        void setDiffuse(const glm::vec3& dif);
        void setSpecular(const glm::vec3& spec);

    protected:
        const std::string m_name;
        Shader& m_shader;
    };

    ///////////////////////
    class DirectionalLight: public Light
    {
    public:
        DirectionalLight(const std::string& name, Shader& shader);

        void setDirection(const glm::vec3& dir);
    };

    ///////////////////////
    class PointLight: public Light
    {
    public:
        PointLight(const std::string& name, Shader& shader);

        void setPosition(const glm::vec3& pos);
        void setConstant(float c);
        void setLinear(float l);
        void setQuadratic(float q);
    };

    ///////////////////////
    class SpotLight: public PointLight
    {
    public:
        SpotLight(const std::string& name, Shader& shader);

        void setDirection(const glm::vec3& dir);
        void setCutOff(float c);
        void setOuterCutOff(float o);
    };
}


