#pragma once
#include "Demo.h"
#include "Window.h"


static const std::string FONT = FONTS_PATH + "arial.ttf";

class Demo1 : public Demo
{
public:
    Demo1(ZFX::Camera& camera) : Demo{ camera, "Demo1 - Rendering Text" },
        m_textRenderer{ FONT }
    {
        m_strings.push_back("<Space> : Next demo");
        m_strings.push_back("<Arrow keys> : Move camera");
        m_strings.push_back("<C> : Toggle background colour (black/white)");
        m_strings.push_back("<D> : Next scene (only frame buffers demo)");
        m_strings.push_back("<W> : Toggle wireframe mode");
        m_strings.push_back("<Z> : Reset zoom");
        m_strings.push_back("");
        m_strings.push_back("Mouse wheel : zoom in/out");
        m_strings.push_back("Mouse middle button + mouse movement : turn camera");
    }

    void draw() override
    {
#if 1
        m_textRenderer.drawText(
            "Hello World!",                          // text
            ZFX::Window::width() / 2.0f - 150.0f,    // x-coordinate (pixel coordinates)
            ZFX::Window::height() - 50.0f,           // y-coordinate (pixel coordinates)
            1.0f,                                    // scale
            glm::vec4(0.5f, 0.8f, 0.2f, 0.5f));      // text colour

        float yCoord = ZFX::Window::height() - 100.0f;
        for(const auto& str : m_strings)
        {
            m_textRenderer.drawText(str, 20.0f, yCoord, 0.5f, ZFX::YELLOW);
            yCoord -= 30.0f;
        }
#else // simple performance test
        m_textRenderer.drawText(
                "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Praesent ac justo non elit\n"
                "tempus ultricies ac in augue. Interdum et malesuada fames ac ante ipsum primis in\n"
                "faucibus. Nam ante lacus, mollis a ligula vitae, hendrerit maximus leo. Donec\n"
                "semper, justo at rutrum fringilla, libero libero efficitur sapien, sed semper sem\n"
                "magna eget lorem. Ut gravida volutpat nibh ac auctor. Vestibulum pellentesque ex at\n"
                "libero tempus cursus. Etiam et egestas enim. In pulvinar dui vitae nisl gravida\n"
                "pretium. Nam ultricies egestas dolor sed rutrum. Aliquam laoreet mauris eu quam\n"
                "mattis, eget sollicitudin est convallis. Cras urna libero, viverra at dolor ut,\n"
                "molestie congue sem. Aliquam vel accumsan justo. Interdum et malesuada fames ac\n"
                "ante ipsum primis in faucibus. Proin imperdiet turpis eu dui semper mattis. Duis\n"
                "non euismod augue.\n\n"

                "In sed sapien eget odio fringilla ornare. Fusce in ultricies neque, quis viverra\n"
                "ex. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac\n"
                "turpis egestas. Maecenas sed consequat velit. Nam et augue augue. Aliquam elementum\n"
                "mollis urna, ut tincidunt odio. Nulla efficitur nisl in massa feugiat dignissim.\n"
                "Donec nibh justo, pellentesque id lacinia ut, aliquet sed nibh.\n\n"

                "Integer a ante et sapien iaculis luctus in eget odio. Suspendisse non mattis odio.\n"
                "Curabitur fermentum consectetur lorem, at gravida ipsum ornare quis. Vivamus\n"
                "sagittis sem purus, eget scelerisque felis maximus vel. Nam nec orci a ex varius\n"
                "lacinia. Pellentesque dolor mauris, commodo et sapien condimentum, fermentum ornare\n"
                "tellus. Nullam dictum elit vitae congue maximus. Duis convallis ultrices ante at\n"
                "pretium. Nam ullamcorper nisi et mi tempor, et semper tortor sollicitudin.\n",
            0.0f, ZFX::Window::height() - 40.0f, 1.0f, ZFX::YELLOW);
#endif
    }

private:
    ZFX::TextFreetype m_textRenderer;
    std::vector<std::string> m_strings;
};
