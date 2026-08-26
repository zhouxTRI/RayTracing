#ifndef COLOR_H
#define COLOR_H

#include "interval.h"
#include "vec3.h"


using color=vec3;

inline double linear_to_gamma(double linear_component){             //将线性空间的颜色分量转换为伽马空间的颜色分量，伽马校正，gamma=2.0
    if(linear_component > 0)
        return sqrt(linear_component);                              
    return 0;

}

void write_color(std::ostream& out, const color& pixel_color, int samples_per_pixel){
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();
    //对每个像素的颜色进行采样平均
    auto scale = 1.0 / samples_per_pixel;
    r *= scale;
    g *= scale;
    b *= scale;

    //应用线性空间到伽马空间的转换
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    //将颜色分量映射到[0,255]范围内，并输出到流中
    static const interval intensity(0.000, 0.999);                  //将颜色分量限制在[0,0.999]范围内，避免出现负数和大于1的值

    out << static_cast<int>(256 * intensity.clamp(r)) << ' '        //将像素颜色的三个分量映射到[0,255]范围内，并输出到流中
        << static_cast<int>(256 * intensity.clamp(g)) << ' '        //xyz分量分别对应红绿蓝通道，大小为256，乘以256是为了将[0,0.999]范围内的颜色分量映射到[0,255]范围内，使用static_cast<int>将浮点数转换为整数
        << static_cast<int>(256 * intensity.clamp(b)) << '\n';     
}

#endif