#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"


class material {
    public:
        virtual ~material() = default;

        virtual bool scatter(                                                   //纯虚函数，计算散射光线和衰减系数
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;  
};

class lambertian : public material{
    public:
        lambertian(const color& a) : albedo(a) {}

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
        const override {
            auto scatter_direction = rec.normal + random_unit_vector();         //根据 Lambertian漫反射模型，计算出新的随机方向向量，方向是以交点法向量终点为中心、法向量长度为半径的单位球内的随机方向
            
            if(scatter_direction.near_zero())                                   //如果散射方向向量接近零向量，则将其设置为法向量，避免在计算漫反射时除以0
                scatter_direction = rec.normal;
            
            scattered = ray(rec.p, scatter_direction);                          //创建散射光线，起点为交点，方向为散射方向
            attenuation = albedo;                                               //衰减系数为材质的反射率
            return true;                                                        //返回true，表示散射成功
        }

    private:
        color albedo;                       //漫反射材质的反射率，表示物体表面反射光线的能力，范围为[0,1]，0表示完全吸收光线，1表示完全反射光线
};

class metal : public material{
    public:
        metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1.0) {}

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
        const override {
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);          
             
            scattered = ray(rec.p, reflected + fuzz * random_unit_vector());  //散射光线的方向为:反射向量再在终点加上一个随机向量，随机向量的长度为fuzz                           
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);  //如果散射光线的方向与法向量的点积大于0，说明散射光线在物体表面上方，返回true，否则返回false,表示散射光线在物体表面下方，无法被观察到
        }
    private:
        color albedo;                       //金属材质的反射率，表示物体表面反射光线的能力，范围为[0,1]，0表示完全吸收光线，1表示完全反射光线
        double fuzz;                        //金属表面的粗糙度，范围为[0,1]，0表示完全光滑，1表示完全粗糙，粗糙度越大，散射光线的方向越随机，反射光线的方向越不确定
};

#endif  