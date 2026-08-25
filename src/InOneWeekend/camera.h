#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"
#include "color.h"
#include "hittable.h"

#include <iostream>

class camera{
    public:
        double aspect_ratio = 1.0;                  //图像宽高比
        int image_width = 100;                      //图像宽度
        int samples_per_pixel = 10;                 //每个像素的采样次数

        void render(const hittable& world){
            initialize();

            std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";                 //输出PPM格式的头部信息

            for(int j=0;j < image_height; j++)
            {
                std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
                for(int i = 0; i < image_width; i++)
                {
                    //auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);    //计算像素中心点在世界坐标系中的位置
                    //auto ray_direction = pixel_center - center;                                     //计算射线方向向量
                    //ray r(center, ray_direction);                                                   //创建一条从相机中心指向像素中心点的射线

                    color pixel_color(0, 0, 0);                                                //计算像素颜色
                    for(int sample = 0; sample < samples_per_pixel; sample++)
                    {
                        ray r = get_ray(i, j);                                                       
                        pixel_color += ray_color(r, world);
                    }
                    write_color(std::cout, pixel_color, samples_per_pixel);                                             //将像素颜色输出到流
                }
            }
            std::clog << "\nDone.       \n";
        }
    private:
        int image_height;                           //图像高度    
        point3 center;                              //相机中心
        point3 pixel00_loc;                         //(0,0)像素中心点在世界坐标系中的位置
        vec3 pixel_delta_u;
        vec3 pixel_delta_v;

        void initialize(){
            image_height = static_cast<int>(image_width / aspect_ratio);                   //根据宽高比计算图像高度
            image_height = (image_height < 1 ) ? 1 : image_height;                          
            center = point3(0, 0, 0);                                                      //相机位置

            //确定viewport
            auto focal_length = 1.0;                                                       //焦距
            auto viewport_height = 2.0;
            auto viewport_width = viewport_height * (double(image_width) / image_height);  //根据图像宽高比计算视口宽度

                //计算水平和垂直视口边缘上的向量
            auto viewport_u = vec3(viewport_width, 0, 0);                                   //水平向量
            auto viewport_v = vec3(0, -viewport_height, 0);                                 //camera空间坐标系y轴向上，图像坐标y轴向下，所以这里取负值
                //计算每个像素的宽度和高度
            pixel_delta_u = viewport_u / image_width;
            pixel_delta_v = viewport_v / image_height;
                //计算视口左上角的点
            auto viewport_upper_left = 
                center -vec3(0,0,focal_length) - viewport_u/2 - viewport_v/2;
            pixel00_loc = viewport_upper_left + 0.5*(pixel_delta_u + pixel_delta_v);       //计算(0,0)像素中心点在世界坐标系中的位置

        }
        ray get_ray(int i, int j) const{                                                    //获得一条从相机中心指向像素中心点的射线
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);    //计算像素中心点在世界坐标系中的位置
            auto pixel_sample = pixel_center + pixel_sample_square();                   //在像素内随机采样一个点，增加抗锯齿效果

            auto ray_origin = center;                                                       //射线起点为相机中心
            auto ray_direction = pixel_sample - ray_origin;                                 //计算射线方向向量
            return ray(ray_origin, ray_direction);                                              //创建一条从相机中心指向
        }

        vec3 pixel_sample_square() const{                           //返回一个在像素内随机采样的点，范围为[-0.5,0.5) * pixel_delta_u + [-0.5,0.5) * pixel_delta_v
            auto px = -0.5 + random_double();                       //在[-0.5,0.5)范围内随机采样一个点
            auto py = -0.5 + random_double();
            return (px * pixel_delta_u) + (py * pixel_delta_v);     //将采样点映射到像素内
        }

        //计算射线与物体的交点，并返回像素颜色
        color ray_color(const ray& r, const hittable& world) const {
            hit_record rec;
            if(world.hit(r, interval(0, infinity), rec))
            {
                return 0.5*(rec.normal + color(1,1,1));                                 //如果射线与物体相交，返回法向量映射到[0,1]范围内的颜色值，形成渐变效果
            }

            vec3 unit_direction = unit_vector(r.direction());                       //将射线方向向量归一化
            auto a = 0.5*(unit_direction.y() + 1.0);                                //将y分量映射到[0,1]范围内,为什么是0.5*(y+1)?因为y分量的范围是[-1,1],所以加1后范围是[0,2],再乘以0.5后范围是[0,1]
            return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);           //线性插值，返回背景颜色，形成渐变效果,blendedValue = (1-a)*startValue + a*endValue, startValue是color(1.0, 1.0, 1.0), endValue是color(0.5, 0.7, 1.0)
        }

};

#endif