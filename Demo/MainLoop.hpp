#pragma once
#include "Camera.h"
#include "Window.h"
#include "Demo.h"
#include <ZFX.h>
#include <chrono>

class MainLoop
{
    using DemoList = std::vector<std::unique_ptr<Demo>>;
    using DemoIterator = DemoList::iterator;

    //using clock = std::chrono::high_resolution_clock;
    using clock = std::chrono::steady_clock;

    struct FramerateControl
    {
        // Time for a frame
        std::chrono::nanoseconds frameLength;

        // When the next frame is expected
        std::chrono::time_point<clock> nextFrame;

        // Start skipping frames when we fall this many frames behind target
        int skipFramesBehind;

        // Max number of frames to skip before giving up and just resync
        int maxFrameSkip;

        // Number of frames skipped
        int framesSkipped;
    };

public:
    MainLoop(ZFX::Window::Options& opts);

    void regulateFramerate();

private:
    void addDemos();
    void loopActions();
    void doFrame(bool draw, const std::chrono::duration<float>& deltaTime);
    void update(float deltaTime);
    void render();
    void handleEvents(float deltaTime);

    void toggleWireframe();
    void changeActiveDemo();
    void checkKeyboardInput(const SDL_Event& e, float deltaTime);
    void checkMouseInput(const SDL_Event& e);

    ZFX::Window m_window;
    ZFX::Camera m_camera;
    bool m_exitRequested;
    FramerateControl m_frameRate;

    DemoList m_demos;
    DemoIterator m_activeDemo;
    bool m_drawFilled;
    bool m_middleButtonDown;
};
