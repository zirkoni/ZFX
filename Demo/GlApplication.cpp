#include "GlApplication.hpp"
#include "MainLoop.hpp"
#include <iostream>

void GlApplication::handleArguments(int argc, char* argv[])
{
    // Nothing...
}

void GlApplication::determineDesktopResolution(int& width, int& height)
{
    ZFX::Window::Options windowOpts{};
    windowOpts.visible = false;
    ZFX::Window tmpWindow(windowOpts);
    const SDL_DisplayMode mode = tmpWindow.getNativeDisplayMode();
    width = mode.w;
    height = mode.h;
}

void GlApplication::run(int argc, char* argv[])
{
    ZFX::Window::Options wOpts;
    wOpts.width = 800;
    wOpts.height = 600;
    wOpts.title = "Demo";
    wOpts.enableVsync = true;
    wOpts.enableGlDebug = true;
    //wOpts.fullscreen = true; // Use the resolution that was configured
    //wOpts.fullscreenDesktop = true; // Use the desktop resolution

    handleArguments(argc, argv);
    //determineDesktopResolution(w, h);

    try
    {
        MainLoop loop(wOpts);
        loop.regulateFramerate();
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
}
