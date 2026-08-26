#include "rtweekend.h"
#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"


// double hit_sphere(const point3& center, double radius, const ray& r){          //判断射线是否与球相交
//     vec3 oc = center - r.origin();                                             //计算球心到射线起点的向量
//     auto a = r.direction().length_squared();                             
//     auto halfb = dot(oc, r.direction());                                    
//     auto c = oc.length_squared() - radius*radius;                                  //计算判别式的各项,用简化的二次方程形式，减少计算量
//     auto discriminant = halfb*halfb - a*c;                                         //计算判别式
//     if(discriminant < 0 )
//     {
//         return -1.0;                                                              //如果判别式小于0，说明射线与球没有交点
//     }else{
//         return (halfb - sqrt(discriminant)) / a;                                 //返回射线与球的交点距离
//     }
// }


int main(){
    //world
    hittable_list world;                                                    //创建一个hittable_list对象，存储所有的hittable对象

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.7, 0.3, 0.3));
    auto material_left =  make_shared<metal>(color(0.8, 0.8, 0.8), 0.3);
    auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));  //添加一个球体，球心位于(0,-100.5,-1)，半径为100，材质为lambertian
    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0),      0.5, material_center));  
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0),     0.5, material_left));
    world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0),      0.5, material_right));
//world.add(make_shared<sphere>(point3(0,0,-1), 0.5));                    //添加一个球体，球心位于(0,0,-1)，半径为0.5
//world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

    //camera
    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;                                             //设置图像宽高比
    cam.image_width = 400;                                                     //设置图像宽度
    cam.samples_per_pixel = 100;                                               //设置每个像素的采样次数
    cam.max_depth = 50;                                                        //设置漫反射计算最大递归深度

    //render
    cam.render(world);
}