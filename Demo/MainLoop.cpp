#include "MainLoop.hpp"
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
#include "Demo7A_ObjectLoader.h"
#include "Demo7B_Backpack.h"
#include "Demo8_Instancing.h"
#include "Demo9_Mouse.h"
#include <thread>

MainLoop::MainLoop(ZFX::Window::Options& opts):
    m_window(opts),
    m_camera(glm::vec3{0.0f, 0.0f, 3.0f}, m_window.aspectRatio()),
    m_exitRequested(false),
    m_drawFilled(true),
    m_middleButtonDown(false)
{
    const unsigned targetFps = 60;
    const float fps = 1.0f / targetFps;
    const auto frameDuration = std::chrono::duration<float>(fps);

    m_frameRate.frameLength = std::chrono::round<std::chrono::nanoseconds>(frameDuration);
    m_frameRate.skipFramesBehind = 3;
    m_frameRate.maxFrameSkip = 3;
    m_frameRate.framesSkipped = 0;

    SDL_DisplayMode mode = m_window.getCurrentDisplayMode();
    std::cout << "Created window with resolution "
              << mode.w << "x" << mode.h << "@" << targetFps << "\n";
}

void MainLoop::addDemos()
{
    m_demos.push_back(std::make_unique<Demo1>(m_window, m_camera));
    m_demos.push_back(std::make_unique<Demo2>(m_window, m_camera));
    m_demos.push_back(std::make_unique<Demo3>(m_window, m_camera));
    m_demos.push_back(std::make_unique<Demo4>(m_window, m_camera));
    m_demos.push_back(std::make_unique<Demo5>(m_window, m_camera));
    m_demos.push_back(std::make_unique<Demo6A>(m_window, m_camera));
    m_demos.push_back(std::make_unique<Demo6B>(m_window, m_camera));
    m_demos.push_back(std::make_unique<Demo6C>(m_window, m_camera));
    m_demos.push_back(std::make_unique<Demo6D>(m_window, m_camera));
    m_demos.push_back(std::make_unique<Demo6E>(m_window, m_camera));
    m_demos.push_back(std::make_unique<Demo6E_MSAA>(m_window, m_camera));
    m_demos.push_back(std::make_unique<Demo6F>(m_window, m_camera));
    m_demos.push_back(std::make_unique<Demo6G>(m_window, m_camera));
    m_demos.push_back(std::make_unique<Demo7A>(m_window, m_camera));
    m_demos.push_back(std::make_unique<Demo7B>(m_window, m_camera));
    m_demos.push_back(std::make_unique<Demo8>(m_window, m_camera));
    m_demos.push_back(std::make_unique<Demo9>(m_window, m_camera));
}

void MainLoop::regulateFramerate()
{
    addDemos();
    m_activeDemo = m_demos.begin();
    m_activeDemo->get()->onEntry();
    m_window.setTitle(m_activeDemo->get()->name());

    while(!m_exitRequested)
    {
        loopActions();
    }
}

void MainLoop::loopActions()
{
    using namespace std::chrono_literals;

    // Get time between next frame and now
    const auto now = clock::now();
    const std::chrono::duration<float> diff = now - m_frameRate.nextFrame;

    if(diff < m_frameRate.frameLength)
    {
        std::this_thread::sleep_for(100ns);
        return;
    }

    // Increment time counter
    m_frameRate.nextFrame += m_frameRate.frameLength;

    // If we have fallen too far behind we need to start skipping frames to catch up
    if(diff > (m_frameRate.frameLength * m_frameRate.skipFramesBehind))
    {
        // If we have already skipped the maximum number of times.  "Give up" and just resync
        if(m_frameRate.framesSkipped >= m_frameRate.maxFrameSkip)
        {
            m_frameRate.nextFrame = clock::now();
            m_frameRate.framesSkipped = 0;
        } else
        {
            // Otherwise skip this frame
            ++m_frameRate.framesSkipped;
            // "Skip" => run logic, but don't draw
            doFrame(false, m_frameRate.frameLength);
            return;
        }
    }

    // Do normal frame
    m_frameRate.framesSkipped = 0;
    doFrame(true, diff);
}

void MainLoop::doFrame(bool draw, const std::chrono::duration<float>& deltaTime)
{
    update(deltaTime.count());

    if(draw)
    {
        render();
    }
}

void MainLoop::update(float deltaTime)
{
    handleEvents(deltaTime);
    m_activeDemo->get()->update();
}

void MainLoop::toggleWireframe()
{
    m_drawFilled = !m_drawFilled;

    if(!m_drawFilled)
    {
        ZFX::wireframeMode();
    } else
    {
        ZFX::filledMode();
    }
}

void MainLoop::changeActiveDemo(bool next)
{
    m_activeDemo->get()->onExit();

    if (next)
    {
        ++m_activeDemo;
        if (m_activeDemo == m_demos.end())
        {
            m_activeDemo = m_demos.begin();
        }
    } else
    {
        if (m_activeDemo == m_demos.begin())
        {
            m_activeDemo = m_demos.end();
        }
        --m_activeDemo;
    }

    m_activeDemo->get()->onEntry();
}

void MainLoop::checkKeyboardInput(const SDL_Event& e, float deltaTime)
{
    const float cameraSpeed = 1.0f;

    if (e.key.keysym.scancode == SDL_SCANCODE_W) // Toggle wireframe mode on/off by pressing W
    {
        toggleWireframe();
    }
    else if (e.key.keysym.scancode == SDL_SCANCODE_SPACE) // Switch to the next demo by pressing space
    {
        changeActiveDemo(true);
        m_window.setTitle(m_activeDemo->get()->name());
    }
    else if (e.key.keysym.scancode == SDL_SCANCODE_BACKSPACE) // Switch to the previous demo by pressing backspace
    {
        changeActiveDemo(false);
        m_window.setTitle(m_activeDemo->get()->name());
    }
    else if (e.key.keysym.scancode == SDL_SCANCODE_Z)
    {
        m_camera.resetZoom();
    }
    else if (e.key.keysym.scancode == SDL_SCANCODE_C) // Change background colour black <=> white
    {
        m_activeDemo->get()->changeBgColour();
    }
    else if (e.key.keysym.scancode == SDL_SCANCODE_UP)
    {
        m_camera.move(ZFX::Camera::Direction::FORWARD, deltaTime, cameraSpeed);
    }
    else if (e.key.keysym.scancode == SDL_SCANCODE_DOWN)
    {
        m_camera.move(ZFX::Camera::Direction::BACKWARD, deltaTime, cameraSpeed);
    }
    else if (e.key.keysym.scancode == SDL_SCANCODE_LEFT)
    {
        m_camera.move(ZFX::Camera::Direction::LEFT, deltaTime, cameraSpeed);
    }
    else if (e.key.keysym.scancode == SDL_SCANCODE_RIGHT)
    {
        m_camera.move(ZFX::Camera::Direction::RIGHT, deltaTime, cameraSpeed);
    } else
    {
        m_activeDemo->get()->handleInput(e);
    }
}

void MainLoop::checkMouseInput(const SDL_Event& e)
{
    if (e.type == SDL_MOUSEWHEEL)
    {
        if (e.wheel.y > 0)
        {
            m_camera.zoomIn();
        } else if (e.wheel.y < 0)
        {
            m_camera.zoomOut();
        }
    } else if (e.type == SDL_MOUSEBUTTONDOWN)
    {
        if (e.button.button == SDL_BUTTON_MIDDLE) m_middleButtonDown = true;
    } else if (e.type == SDL_MOUSEBUTTONUP)
    {
        if (e.button.button == SDL_BUTTON_MIDDLE) m_middleButtonDown = false;
    } else if (e.type == SDL_MOUSEMOTION)
    {
        if (m_middleButtonDown)
        {
            m_camera.turn(e.motion.xrel, e.motion.yrel);
        }
    }
}

void MainLoop::handleEvents(float deltaTime)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
            {
                m_exitRequested = true;
                break;
            }
            case SDL_KEYDOWN:
            {
                checkKeyboardInput(event, deltaTime);
                break;
            }
            case SDL_MOUSEMOTION:
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
            case SDL_MOUSEWHEEL:
            {
                checkMouseInput(event);
                break;
            }
            {
                break;
            }
            case SDL_WINDOWEVENT:
            {
                break;
            }
            default:
            {
                break;
            }
        }
    }
}

void MainLoop::render()
{
    m_window.clear(m_activeDemo->get()->bgColour());
    m_activeDemo->get()->draw();
    m_window.update();
}
