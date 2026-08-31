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

class dielectric : public material{
    public:
        dielectric(double index_of_refraction) : ir(index_of_refraction) {}   

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
        const override {
            attenuation = color(1.0,1.0,1.0);                           //衰减系数为1，表示光线在介质中传播时不会被吸收
            double refraction_ratio = rec.front_face ? (1.0/ir) : ir;   //如果是正面交点，折射率为1/ir，否则为ir：因为光线从空气进入介质时，折射率为1/ir，从介质进入空气时，折射率为ir
            vec3 unit_direction = unit_vector(r_in.direction());            
            double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);  //计算入射角的余弦值，fmin是为了避免浮点数误差导致的cos_theta大于1
            double sin_theta = sqrt(1.0 - cos_theta*cos_theta);
            bool cannot_refract = refraction_ratio * sin_theta > 1.0;   //如果折射率乘以入射角的正弦值大于1，说明光线无法折射，发生全反射
            vec3 direction;

            if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()){      //随机数是为了模拟光线在介质界面上的反射和折射的概率分布，增加渲染的真实感
                direction = reflect(unit_direction, rec.normal);                    //反射光线的方向向量
            }else{
                direction = refract(unit_direction, rec.normal, refraction_ratio);  //折射光线的方向向量
            }
            
            scattered = ray(rec.p, direction);  
            return true;
        }

    private:
        double ir; // Index of Refraction 折射率,表示光线在不同介质中的传播速度的比值，范围为[1,∞)，空气的折射率为1.0，水的折射率为1.33，玻璃的折射率为1.5，钻石的折射率为2.42    

        static double reflectance(double cosine, double ref_idx){  //Schlick's approximation 计算反射率的近似值，cosine为入射角的余弦值，ref_idx为折射率
            auto r0 = (1-ref_idx) / (1+ref_idx);                    //计算反射率的近似值
            r0 = r0*r0;                                             
            return r0 + (1-r0)*pow((1-cosine),5);                   //返回反射率的近似值,当该值大于随机数时，说明光线发生反射，否则发生折射；
        }                                                           //Schlick's approximation是一个经验公式，用于计算光线在不同介质界面上的反射率，能够在不进行复杂的光学计算的情况下，快速地估计反射率。
};

#endif  