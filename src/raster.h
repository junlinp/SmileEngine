#ifndef SRC_RASTER_H_
#define SRC_RASTER_H_

#include "triangle.h"

class Raster {
public:
static std::vector<std::pair<int, int>> RasterNDCTriangleMask(const NDCTriangle& ndc_angle, size_t width, size_t height);
};
#endif // SRC_RASTER_H_