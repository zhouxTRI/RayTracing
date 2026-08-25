#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include <iostream>

using color=vec3;

void write_color(std::ostream& out, const color& pixel_color, int samples_per_pixel){
    auto r = pixel_color.x();
    auto b = pixel_color.y();
    auto g = pixel_color.z();
    //对每个像素的颜色进行采样平均
    auto scale = 1.0 / samples_per_pixel;
    r *= scale;
    g *= scale;
    b *= scale;

    //将颜色分量映射到[0,255]范围内，并输出到流中
    static const interval intensity(0.000, 0.999);                  //将颜色分量限制在[0,0.999]范围内，避免出现负数和大于1的值

    out << static_cast<int>(256 * intensity.clamp(r)) << ' '        //将像素颜色的三个分量映射到[0,255]范围内，并输出到流中
        << static_cast<int>(256 * intensity.clamp(g)) << ' '        //xyz分量分别对应红绿蓝通道，大小为256，乘以256是为了将[0,0.999]范围内的颜色分量映射到[0,255]范围内，使用static_cast<int>将浮点数转换为整数
        << static_cast<int>(256 * intensity.clamp(b)) << '\n';     
}

#endif