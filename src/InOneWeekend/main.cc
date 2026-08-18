#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>

double hit_sphere(const point3& center, double radius, const ray& r){          //判断射线是否与球相交
    vec3 oc = center - r.origin();                                             //计算球心到射线起点的向量
    auto a = r.direction().length_squared();                             
    auto halfb = dot(oc, r.direction());                                    
    auto c = oc.length_squared() - radius*radius;                                  //计算判别式的各项,用简化的二次方程形式，减少计算量
    auto discriminant = halfb*halfb - a*c;                                         //计算判别式

    if(discriminant < 0 )
    {
        return -1.0;                                                              //如果判别式小于0，说明射线与球没有交点
    }else{
        return (halfb - sqrt(discriminant)) / a;                                 //返回射线与球的交点距离
    }
   
}

color ray_color(const ray& r){
    //在视口Z轴上放一个球,球心位于(0,0,-1)，如果射线与球相交，返回红色，否则返回背景颜色
    auto t = hit_sphere(point3(0,0,-1), 0.5, r);                                   
    if(t > 0.0){
        vec3 N = unit_vector(r.at(t) - point3(0,0,-1));                     //计算射线与球的交点的法向量
        return 0.5*color(N.x() + 1.0, N.y() + 1.0, N.z() + 1.0);            //如果射线与球相交，返回球的法向量映射到[0,1]范围内的颜色值，形成渐变效果
    }

    vec3 unit_direction = unit_vector(r.direction());                       //将射线方向向量归一化
    auto a = 0.5*(unit_direction.y() + 1.0);                                //将y分量映射到[0,1]范围内,为什么是0.5*(y+1)?因为y分量的范围是[-1,1],所以加1后范围是[0,2],再乘以0.5后范围是[0,1]
    return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);           //线性插值，返回背景颜色，形成渐变效果,blendedValue = (1-a)*startValue + a*endValue, startValue是color(1.0, 1.0, 1.0), endValue是color(0.5, 0.7, 1.0)
}

int main(){
    //image
    auto aspect_ratio = 16.0/9.0;                                           //宽高比
    int image_width = 400;                                                  //图像宽度
    int image_height = static_cast<int>(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;                   //确保图像高度至少为1
    //camera
    auto focal_length = 1.0;                                                //焦距
    auto viewport_height = 2.0;                                             //视口高度
    auto viewport_width = viewport_height * (double(image_width) / image_height);
    auto camera_center = point3(0, 0, 0);                                   //相机位置
        //计算水平和垂直视口边缘上的向量
    auto viewport_u = vec3(viewport_width, 0, 0);                           //水平向量
    auto viewport_v = vec3(0, -viewport_height, 0);                         //camera空间坐标系y轴向上，图像坐标y轴向下，所以这里取负值
        //计算每个像素的宽度和高度
    auto pixel_delta_u = viewport_u / image_width;                                         
    auto pixel_delta_v = viewport_v / image_height;
        //计算视口左上角的点
    auto viewport_upper_left = camera_center -vec3(0,0,focal_length) - viewport_u/2 - viewport_v/2; 
    auto pixel00_loc = viewport_upper_left + 0.5*(pixel_delta_u + pixel_delta_v);  

    //render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for(int j=0; j < image_height; ++j){

        std::clog << "\rScanlines remaining: " << image_height - j - 1 << ' ' << std::flush;

        for(int i=0; i < image_width; ++i){
            auto pixel_center = pixel00_loc + i*pixel_delta_u + j*pixel_delta_v;  //计算像素中心点在世界坐标系中的位置
            auto ray_direction = pixel_center - camera_center;  //计算射线方向向量
            ray r(camera_center, ray_direction);    
            
            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);                                                    //将像素颜色输出到流
        }
    }
    std::clog << "\nDone.       \n";
}