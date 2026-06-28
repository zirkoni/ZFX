#pragma once

class GlApplication
{
public:
    GlApplication() = default;
    ~GlApplication() = default;

    void run(int argc, char* argv[]);

private:
    void handleArguments(int argc, char* argv[]);
    void determineDesktopResolution(int& width, int& height);
};
