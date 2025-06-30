#include "Demo1_Text_Freetype.h"
#include "Demo2_Triangles.h"
#include "Demo3_Square.h"
#include "Demo4_Circles.h"
#include "Demo5_Cube.h"
#include "Demo6A_BasicLighting.h"
#include "Demo6B_Materials.h"
#include "Demo6C_LightMaps.h"
#include "Demo6D_LightCasters.h"
#include "Demo6E_FrameBuffers.h"
#include "Demo6E_MSAA_FrameBuffers.h"
#include "Demo6F_ShadowMapping.h"
#include "Demo6G_PointShadows.h"
#include "Demo7_ObjectLoader.h"
#include "Demo8_Instancing.h"
#include <iostream>
#include <vector>


static bool drawFilled = true;

using DemoList = std::vector<std::unique_ptr<Demo> >;

void addDemos(DemoList& demos, ZFX::Window& window, ZFX::Camera& camera)
{
    demos.push_back(std::make_unique<Demo1>(window, camera));
    demos.push_back(std::make_unique<Demo2>(window, camera));
    demos.push_back(std::make_unique<Demo3>(window, camera));
    demos.push_back(std::make_unique<Demo4>(window, camera));
    demos.push_back(std::make_unique<Demo5>(window, camera));
    demos.push_back(std::make_unique<Demo6A>(window, camera));
    demos.push_back(std::make_unique<Demo6B>(window, camera));
    demos.push_back(std::make_unique<Demo6C>(window, camera));
    demos.push_back(std::make_unique<Demo6D>(window, camera));
    demos.push_back(std::make_unique<Demo6E>(window, camera));
    demos.push_back(std::make_unique<Demo6E_MSAA>(window, camera));
    demos.push_back(std::make_unique<Demo6F>(window, camera));
    demos.push_back(std::make_unique<Demo6G>(window, camera));
//    demos.push_back(std::make_unique<Demo7>(window, camera)); // Very slow to load on Intel integrated GPU/mobile CPU! TODO: fix
    demos.push_back(std::make_unique<Demo8>(window, camera));
}

void toggleWireframe()
{
    drawFilled = !drawFilled;

    if(!drawFilled)
    {
        ZFX::wireframeMode();
    } else
    {
        ZFX::filledMode();
    }
}

void changeActiveDemo(ZFX::Camera& camera, DemoList& demos, DemoList::iterator& activeDemo)
{
    activeDemo->get()->onExit();

    ++activeDemo;
    if (activeDemo == demos.end())
    {
        activeDemo = demos.begin();
    }

    activeDemo->get()->onEntry();
}

void checkKeyboardInput(const SDL_Event& e, ZFX::Camera& camera, DemoList& demos,
        DemoList::iterator& activeDemo, float deltaTime, ZFX::Window& window)
{
    if (e.key.keysym.scancode == SDL_SCANCODE_W) // Toggle wireframe mode on/off by pressing W
    {
        toggleWireframe();
    }
    else if (e.key.keysym.scancode == SDL_SCANCODE_SPACE) // Switch to different demo by pressing space
    {
        changeActiveDemo(camera, demos, activeDemo);
        window.setTitle(activeDemo->get()->name());
    }
    else if (e.key.keysym.scancode == SDL_SCANCODE_Z)
    {
        camera.resetZoom();
    }
    else if (e.key.keysym.scancode == SDL_SCANCODE_C) // Change background colour black <=> white
    {
        activeDemo->get()->changeBgColour();
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
    } else
    {
        activeDemo->get()->handleInput(e);
    }
}

void checkMouseInput(const SDL_Event& e, ZFX::Camera& camera)
{
    static bool middleButtonDown = false;

    if (e.type == SDL_MOUSEWHEEL)
    {
        if (e.wheel.y > 0)
        {
            camera.zoomIn();
        } else if (e.wheel.y < 0)
        {
            camera.zoomOut();
        }
    } else if (e.type == SDL_MOUSEBUTTONDOWN)
    {
        if (e.button.button == SDL_BUTTON_MIDDLE) middleButtonDown = true;
    } else if (e.type == SDL_MOUSEBUTTONUP)
    {
        if (e.button.button == SDL_BUTTON_MIDDLE) middleButtonDown = false;
    } else if (e.type == SDL_MOUSEMOTION)
    {
        if (middleButtonDown)
        {
            camera.turn(e.motion.xrel, e.motion.yrel);
        }
    }
}

bool checkInput(ZFX::Camera& camera, DemoList& demos, DemoList::iterator& activeDemo, float deltaTime, ZFX::Window& window)
{
    SDL_Event e;

    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            return true;
        } else if (e.type == SDL_KEYDOWN)
        {
            checkKeyboardInput(e, camera, demos, activeDemo, deltaTime, window);
        } else
        {
            checkMouseInput(e, camera);
        }
    }

    return false;
}

void mainLoop(ZFX::Window& window)
{
    ZFX::Camera camera{ glm::vec3{0.0f, 0.0f, 3.0f}, window.aspectRatio() };

    DemoList demos;
    addDemos(demos, window, camera);
    auto activeDemo = demos.begin();
    window.setTitle(activeDemo->get()->name());

    bool exitRequested = false;

    Uint64 now = SDL_GetPerformanceCounter();
    Uint64 last = 0;
    float deltaTime = 0.0f;

    while (!exitRequested)
    {
        last = now;
        now = SDL_GetPerformanceCounter();
        deltaTime = (now - last) * 1000 / (float)SDL_GetPerformanceFrequency();

        exitRequested = checkInput(camera, demos, activeDemo, deltaTime, window);

        window.clear(activeDemo->get()->bgColour());
        activeDemo->get()->draw();
        window.update();

        SDL_Delay(1);
    }
}

int main(int argc, char* argv[])
{
    try
    {
        ZFX::Window::Options wOpts;
        wOpts.width = 800;
        wOpts.height = 600;
        wOpts.title = "Demo";
        wOpts.enableVsync = true;
        wOpts.enableGlDebug = true;
        //wOpts.fullscreen = true; // Use the resolution that was configured
        //wOpts.fullscreenDesktop = true; // Use the desktop resolution

        ZFX::Window window{ wOpts };
        SDL_DisplayMode mode = window.getCurrentDisplayMode();
        std::cout << "Created window with resolution " << mode.w << "x" << mode.h << "\n";

        mainLoop(window);
    } catch (const ZFX::Exception& e)
    {
        std::cerr << "ZFX::Exception:\n" << e.what() << "\n";
    } catch(const std::exception& e)
    {
        std::cerr << "std::exception:\n" << e.what() << "\n";
    } catch(...)
    {
        std::cerr << "Unknown exception occurred\n";
    }

    return 0;
}
