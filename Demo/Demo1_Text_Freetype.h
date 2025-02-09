#pragma once
#include "Demo.h"
#include "Window.h"


class Demo1 : public Demo
{
    const std::string FONT = FONTS_PATH + "arial.ttf";

public:
    Demo1(ZFX::Window& window, ZFX::Camera& camera) : Demo{ window, camera, "Demo1 - Rendering Text" },
        m_textRenderer{ FONT, window.width(), window.height() }
    {
        m_helpText = "<Space> : Next demo\n"
                     "<Arrow keys> : Move camera\n"
                     "<C> : Toggle background colour (black/white)\n"
                     "<D> : Next scene (only frame buffers demo)\n"
                     "<W> : Toggle wireframe mode\n"
                     "<Z> : Reset zoom\n\n"
                     "Mouse wheel : zoom in/out\n"
                     "Mouse middle button + mouse movement : turn camera";
    }

    void draw() override
    {
        glDisable(GL_DEPTH_TEST);
#if 1
        m_textRenderer.drawText(
            "Hello World!",                      // text
            m_window.width() / 2.0f - 150.0f,    // x-coordinate (pixel coordinates)
            m_window.height() - 50.0f,           // y-coordinate (pixel coordinates)
            0.7f,                                // scale
            glm::vec4(0.5f, 0.8f, 0.2f, 0.5f));  // text colour

        float yCoord = m_window.height() - 100.0f;
        m_textRenderer.drawText(m_helpText, 20.0f, yCoord, 0.4f, ZFX::YELLOW);
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
            0.0f, m_window.height() - 40.0f, 0.3f, ZFX::YELLOW);
#endif
        glEnable(GL_DEPTH_TEST);
    }

private:
    ZFX::TextFreetype m_textRenderer;
    std::string m_helpText;
};
