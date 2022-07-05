#pragma once
#include "ZFX.h"
#include "Constants.h"


class Demo
{
public:
    Demo(ZFX::Camera& camera, const std::string& name): m_counter{ 0.0f }, m_camera{ camera }, m_name{ name } {}
    virtual ~Demo() {}

    //virtual void update(float deltaTime) = 0; // TODO
    virtual void draw() = 0;

    const std::string& name() { return m_name; }

protected:
    float m_counter;
    ZFX::Camera& m_camera;
    std::string m_name;
};
