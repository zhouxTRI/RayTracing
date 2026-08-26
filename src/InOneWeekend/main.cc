#include "rtweekend.h"
#include "camera.h"
#include "hittable_list.h"
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
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));                    //添加一个球体，球心位于(0,0,-1)，半径为0.5
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

    //camera
    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;                                             //设置图像宽高比
    cam.image_width = 400;                                                     //设置图像宽度
    cam.samples_per_pixel = 100;                                               //设置每个像素的采样次数

    //render
    cam.render(world);
}