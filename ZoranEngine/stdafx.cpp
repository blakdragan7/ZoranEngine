#include "stdafx.h"
#define LOADBMP_IMPLEMENTATION
#include <ThirdParty/loadbmp/loadbmp.hpp>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG
#define STBI_ONLY_BMP
#define STBI_ONLY_TGA
#include <ThirdParty/stb/stb_image.h>