#include "glDebug.h"
#include <string>
#include <GL/glew.h>
#include <SDL2/SDL.h>


namespace ZFX
{
    std::string _source(const GLenum source)
    {
        switch(source)
        {
            case GL_DEBUG_SOURCE_API:               return "GL_DEBUG_SOURCE_API";
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:     return "GL_DEBUG_SOURCE_WINDOW_SYSTEM";
            case GL_DEBUG_SOURCE_SHADER_COMPILER:   return "GL_DEBUG_SOURCE_SHADER_COMPILER";
            case GL_DEBUG_SOURCE_THIRD_PARTY:       return "GL_DEBUG_SOURCE_THIRD_PARTY";
            case GL_DEBUG_SOURCE_APPLICATION:       return "GL_DEBUG_SOURCE_APPLICATION";
            case GL_DEBUG_SOURCE_OTHER:             return "GL_DEBUG_SOURCE_OTHER";
            default:                                return std::to_string(source);
        }
    }

    std::string _type(const GLenum type)
    {
        switch(type)
        {
            case GL_DEBUG_TYPE_ERROR:               return "GL_DEBUG_TYPE_ERROR";
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR";
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  return "GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR";
            case GL_DEBUG_TYPE_PORTABILITY:         return "GL_DEBUG_TYPE_PORTABILITY";
            case GL_DEBUG_TYPE_PERFORMANCE:         return "GL_DEBUG_TYPE_PERFORMANCE";
            case GL_DEBUG_TYPE_OTHER:               return "GL_DEBUG_TYPE_OTHER";
            case GL_DEBUG_TYPE_MARKER:              return "GL_DEBUG_TYPE_MARKER";
            default:                                return std::to_string(type);
        }
    }

    std::string _severity(const GLenum severity)
    {
        switch(severity)
        {
            case GL_DEBUG_SEVERITY_HIGH:            return "GL_DEBUG_SEVERITY_HIGH";
            case GL_DEBUG_SEVERITY_MEDIUM:          return "GL_DEBUG_SEVERITY_MEDIUM";
            case GL_DEBUG_SEVERITY_LOW:             return "GL_DEBUG_SEVERITY_LOW";
            case GL_DEBUG_SEVERITY_NOTIFICATION:    return "GL_DEBUG_SEVERITY_NOTIFICATION";
            default:                                return std::to_string(severity);
        }
    }

    void GLAPIENTRY
    _debugMsgCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam )
    {
        SDL_Log("[%u/%s] %s (%s): %s", id, _type(type).c_str(), _source(source).c_str(), _severity(severity).c_str(), message);
    }

    void glInfo()
    {
        SDL_Log("#### OpenGL Info ####");
        SDL_Log("GLEW_VERSION: %s", glewGetString(GLEW_VERSION));
        SDL_Log("GL_VERSION: %s", glGetString(GL_VERSION));
        SDL_Log("GL_VENDOR: %s", glGetString(GL_VENDOR));
        SDL_Log("GL_RENDERER: %s", glGetString(GL_RENDERER));
        SDL_Log("GL_SHADING_LANGUAGE_VERSION: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
    }

    void enableGlDebug(bool logNotifications)
    {
        glInfo();
        glEnable(GL_DEBUG_OUTPUT);
//        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

        if(!logNotifications) glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);

        glDebugMessageCallback(_debugMsgCallback, 0);
    }
}




