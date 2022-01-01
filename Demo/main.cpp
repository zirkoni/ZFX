#include "Demo1_Triangles.h"
#include "Demo2_Square.h"
#include "Demo3_Text_Freetype.h"
#include "Demo4_Circles.h"
#include "Demo5_Cube.h"
#include "Demo6_BasicLighting.h"
#include "Demo7_ObjectLoader.h"
#include "Demo8_Instancing.h"
#include <iostream>
#include <vector>


using DemoList = std::vector<std::unique_ptr<Demo> >;

void addDemos(DemoList& demos, ZFX::Camera& camera)
{
    demos.emplace_back(std::make_unique<Demo8>(camera)); // Something is wrong, Demo6 and Demo7 break movement in Demo8.
    demos.emplace_back(std::make_unique<Demo1>(camera));
    demos.emplace_back(std::make_unique<Demo2>(camera));
    demos.emplace_back(std::make_unique<Demo3>(camera));
    demos.emplace_back(std::make_unique<Demo4>(camera));
    demos.emplace_back(std::make_unique<Demo5>(camera));
    demos.emplace_back(std::make_unique<Demo6>(camera));
    demos.emplace_back(std::make_unique<Demo7>(camera));
}

void mainLoop(ZFX::Window& window)
{
    bool wireframeOn = false;
    bool wireframeOnOldValue = wireframeOn;
    auto bgColour = ZFX::BLACK;

    ZFX::Camera camera{ glm::vec3{0.0f, 0.0f, 3.0f}, window.aspectRatio() };

    DemoList demos;
    addDemos(demos, camera);
    auto activeDemo = demos.begin() + 1;

    bool exitRequested = false;
    SDL_Event e;

    while (!exitRequested)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                exitRequested = true;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.scancode == SDL_SCANCODE_W) // Toggle wireframe mode on/off by pressing W
                {
                    wireframeOn = !wireframeOn;
                }
                else if (e.key.keysym.scancode == SDL_SCANCODE_SPACE) // Switch to different demo by pressing space
                {
                    ++activeDemo;
                    if (activeDemo == demos.end())
                    {
                        activeDemo = demos.begin();
                    }

                    camera.resetZoom();
                }
                else if (e.key.keysym.scancode == SDL_SCANCODE_Z)
                {
                    camera.resetZoom();
                }
                else if (e.key.keysym.scancode == SDL_SCANCODE_C) // Change background colour black <=> white
                {
                    if (bgColour == ZFX::BLACK) bgColour = ZFX::WHITE;
                    else bgColour = ZFX::BLACK;
                }
            }
            else if (e.type == SDL_MOUSEWHEEL) // Zoom in/out
            {
                if (e.wheel.y > 0)
                {
                    camera.zoomIn();
                }
                else if (e.wheel.y < 0)
                {
                    camera.zoomOut();
                }
            }
        }

        if (wireframeOn != wireframeOnOldValue)
        {
            if (wireframeOn) ZFX::wireframeMode();
            if (not wireframeOn) ZFX::filledMode();
            wireframeOnOldValue = wireframeOn;
        }

        window.clear(bgColour);

        glClear(GL_DEPTH_BUFFER_BIT);
        activeDemo->get()->draw();

        window.update();

        SDL_Delay(1);
    }
}

int main(int argc, char* argv[])
{
    try
    {
        ZFX::Window window{ 800, 600, "Demo" };
        window.clear();
        window.update();
        mainLoop(window);
    }
    catch (const std::runtime_error& error) // TODO: maybe typedef type to e.g. ZFX::Exception?
    {
        std::cerr << "Runtime error:\n" << error.what() << "\n";
    }

    return 0;
}
