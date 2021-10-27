#include "Demo1_Triangles.h"
#include "Demo2_Square.h"
#include "Demo3_Text_Freetype.h"
#include "Demo4_Circles.h"
#include "Demo5_Cube.h"
#include "Demo6_BasicLighting.h"
#include "Demo7_ObjectLoader.h"
#include <iostream>
#include <vector>


using DemoList = std::vector<std::unique_ptr<Demo> >;

void addDemos(DemoList& demos, ZFX::Camera& camera)
{
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
    ZFX::Camera camera{ glm::vec3{0.0f, 0.0f, 3.0f}, window.aspectRatio() };
    bool wireframeOn = false;
    bool wireframeOnOldValue = wireframeOn;

    DemoList demos;
    addDemos(demos, camera);
    auto activeDemo = demos.begin();

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
                /* Toggle wireframe mode on/off by pressing W */
                if (e.key.keysym.scancode == SDL_SCANCODE_W)
                {
                    wireframeOn = !wireframeOn;
                }
                /* Switch to different demo by pressing space */
                else if (e.key.keysym.scancode == SDL_SCANCODE_SPACE)
                {
                    ++activeDemo;
                    if (activeDemo == demos.end())
                    {
                        activeDemo = demos.begin();
                    }
                }
                else if (e.key.keysym.scancode == SDL_SCANCODE_Z)
                {
                    camera.resetZoom();
                }
            }
            else if (e.type == SDL_MOUSEWHEEL)
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

        window.clear();

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