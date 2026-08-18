#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include <iostream>

using color=vec3;

void write_color(std::ostream& out, color pixel_color){
    out << static_cast<int>(255.999 * pixel_color.x()) << ' '               //将像素颜色的三个分量映射到[0,255]范围内，并输出到流中
        << static_cast<int>(255.999 * pixel_color.y()) << ' '               //xyz分量分别对应红绿蓝通道，大小为[0,1]，乘以255.999后映射到[0,255]范围内
        << static_cast<int>(255.999 * pixel_color.z()) << '\n';             //255.999让最大值尽量接近255，避免出现256的情况（用static_cast<int>转换时会向下取整）
}

#endif