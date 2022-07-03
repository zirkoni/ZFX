#include "Demo1_Triangles.h"
#include "Demo2_Square.h"
#include "Demo3_Text_Freetype.h"
#include "Demo4_Circles.h"
#include "Demo5_Cube.h"
#include "Demo6_BasicLighting.h"
#include "Demo6B_Materials.h"
#include "Demo7_ObjectLoader.h"
#include "Demo8_Instancing.h"
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
    demos.emplace_back(std::make_unique<Demo6B>(camera));
    demos.emplace_back(std::make_unique<Demo7>(camera));
    demos.emplace_back(std::make_unique<Demo8>(camera));
}

void mainLoop(ZFX::Window& window)
{
    bool wireframeOn = false;
    bool wireframeOnOldValue = wireframeOn;
    auto bgColour = ZFX::BLACK;

    ZFX::Camera camera{ glm::vec3{0.0f, 0.0f, 3.0f}, window.aspectRatio() };

    DemoList demos;
    addDemos(demos, camera);
    auto activeDemo = demos.begin();

    bool exitRequested = false;
    bool middleButtonDown = false;
    SDL_Event e;

    Uint64 now = SDL_GetPerformanceCounter();
    Uint64 last = 0;
    float deltaTime = 0.0f;

    while (!exitRequested)
    {
        last = now;
        now = SDL_GetPerformanceCounter();
        deltaTime = (now - last) * 1000 / (float)SDL_GetPerformanceFrequency();

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

                    if (activeDemo->get()->name() == "Demo8")
                    {
                        camera.position().z = 80.0f;
                    } else
                    {
                        camera.position().z = 3.0f;
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
                else if (e.key.keysym.scancode == SDL_SCANCODE_UP)
                {
                    camera.move(ZFX::Camera::Direction::FORWARD, deltaTime);
                }
                else if (e.key.keysym.scancode == SDL_SCANCODE_DOWN)
                {
                    camera.move(ZFX::Camera::Direction::BACKWARD, deltaTime);
                }
                else if (e.key.keysym.scancode == SDL_SCANCODE_LEFT)
                {
                    camera.move(ZFX::Camera::Direction::LEFT, deltaTime);
                }
                else if (e.key.keysym.scancode == SDL_SCANCODE_RIGHT)
                {
                    camera.move(ZFX::Camera::Direction::RIGHT, deltaTime);
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
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                if (e.button.button == SDL_BUTTON_MIDDLE) middleButtonDown = true;
            }
            else if (e.type == SDL_MOUSEBUTTONUP)
            {
                if (e.button.button == SDL_BUTTON_MIDDLE) middleButtonDown = false;
            }
            else if (e.type == SDL_MOUSEMOTION)
            {
                if (middleButtonDown)
                {
                    camera.turn(e.motion.xrel, e.motion.yrel);
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

        activeDemo->get()->draw();

        window.update();

        SDL_Delay(1);
    }
}

int main(int argc, char* argv[])
{
    const bool vsync = true;
    const bool glDebug = true;

    try
    {
        ZFX::Window window{ 800, 600, "Demo", vsync, glDebug };
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
