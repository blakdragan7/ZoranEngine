
#include "save-png.h"

#include "../core/arithmetics.hpp"
#include <ThirdParty/loadpng/lodepng.h>

namespace msdfgen {

bool savePng(const Bitmap<float> &bitmap, const char *filename) {
    std::vector<unsigned char> pixels(bitmap.width()*bitmap.height());
    std::vector<unsigned char>::iterator it = pixels.begin();
    for (int y = bitmap.height()-1; y >= 0; --y)
        for (int x = 0; x < bitmap.width(); ++x)
            *it++ = clamp(int(bitmap(x, y)*0x100), 0xff);
    return !lodepng::encode(filename, pixels, bitmap.width(), bitmap.height(), LCT_GREY);
}

bool savePng(const Bitmap<FloatRGB> &bitmap, const char *filename) {
    std::vector<unsigned char> pixels(3*bitmap.width()*bitmap.height());
    std::vector<unsigned char>::iterator it = pixels.begin();
    for (int y = bitmap.height()-1; y >= 0; --y)
        for (int x = 0; x < bitmap.width(); ++x) {
            *it++ = clamp(int(bitmap(x, y).r*0x100), 0xff);
            *it++ = clamp(int(bitmap(x, y).g*0x100), 0xff);
            *it++ = clamp(int(bitmap(x, y).b*0x100), 0xff);
        }
    return !lodepng::encode(filename, pixels, bitmap.width(), bitmap.height(), LCT_RGB);
}

bool encodePng(unsigned char** data, size_t* size, const Bitmap<FloatRGB> &bitmap) {
	std::vector<unsigned char> pixels(3 * bitmap.width()*bitmap.height());
	std::vector<unsigned char>::iterator it = pixels.begin();
	int inH = bitmap.height() - 1;
	for (int y = bitmap.height() - 1; y >= 0; --y)
		for (int x = 0; x < bitmap.width(); ++x) {
			*it++ = clamp(int(bitmap(x, inH - y).r * 0x100), 0xff);
			*it++ = clamp(int(bitmap(x, inH - y).g * 0x100), 0xff);
			*it++ = clamp(int(bitmap(x, inH - y).b * 0x100), 0xff);
		}
	return !lodepng_encode_memory(data, size, (unsigned char*)&pixels.at(0), bitmap.width(), bitmap.height(), LCT_RGB, 8);
}

bool encodePng(unsigned char** data, size_t* size, const Bitmap<float> &bitmap) {
	std::vector<unsigned char> pixels(3 * bitmap.width()*bitmap.height());
	std::vector<unsigned char>::iterator it = pixels.begin();
	int inH = bitmap.height() - 1;
	for (int y = bitmap.height() - 1; y >= 0; --y)
		for (int x = 0; x < bitmap.width(); ++x) {
			*it++ = clamp(int(bitmap(x, inH - y) * 0x100), 0xff);
		}
	return !lodepng_encode_memory(data, size, (unsigned char*)&pixels.at(0), bitmap.width(), bitmap.height(), LCT_GREY, 8);
}

}
