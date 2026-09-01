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

    /*三球场景
    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left =  make_shared<dielectric>(1.5);
    auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));  //添加一个球体，球心位于(0,-100.5,-1)，半径为100，材质为lambertian
    world.add(make_shared<sphere>(point3(0.0, 0.0,    -1.0),   0.5, material_center));  
    world.add(make_shared<sphere>(point3(-1.0, 0.0,   -1.0),   0.5, material_left));
    world.add(make_shared<sphere>(point3(-1.0, 0.0,   -1.0),  -0.4, material_left));
    world.add(make_shared<sphere>(point3(1.0, 0.0,    -1.0),   0.5, material_right));
    */
    
    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }
    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));
    auto material2 = make_shared<lambertian>(color(0.4,0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));
    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    //camera
    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;                                             //设置图像宽高比
    cam.image_width = 1200;                                                     //设置图像宽度
    cam.samples_per_pixel = 10;                                               //设置每个像素的采样次数
    cam.max_depth = 50;                                                        //设置漫反射计算最大递归深度
    
    cam.vfov = 20;                                                             //设置垂直视场角,大小决定了相机的视野范围，角度越大，视野越广，角度越小，视野越窄
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);
    cam.defocus_angle = 0.6;
    cam.focus_dist = 10;
    //render
    cam.render(world);
}