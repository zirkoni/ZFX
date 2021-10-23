#include "Constants.h"

#ifdef __linux__
const std::string RESOURCES_BASE_PATH = "Demo/";
#else
const std::string RESOURCES_BASE_PATH = "../../../Demo/";
#endif

const std::string SHADERS_PATH = RESOURCES_BASE_PATH + "Shaders/";
const std::string TEXTURES_PATH = RESOURCES_BASE_PATH + "Textures/";
const std::string FONTS_PATH = RESOURCES_BASE_PATH + "Fonts/";
const std::string OBJS_PATH = RESOURCES_BASE_PATH + "Objects/";
