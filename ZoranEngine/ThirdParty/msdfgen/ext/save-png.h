
#pragma once

#include "../core/Bitmap.h"

namespace msdfgen {

/// Saves the bitmap as a PNG file.
bool savePng(const Bitmap<float> &bitmap, const char *filename);
bool savePng(const Bitmap<FloatRGB> &bitmap, const char *filename);
bool encodePng(unsigned char** data, size_t* size, const Bitmap<FloatRGB> &bitmap);
bool encodePng(unsigned char** data, size_t* size, const Bitmap<float> &bitmap);
}
