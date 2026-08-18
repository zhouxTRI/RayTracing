#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
    public:
        sphere(const point3& center, double radius)
        : center(center), radius(std::fmax(0.0, radius)) {}

        bool hit(const ray&r, double t_min, double t_max, hit_record& rec) const override {
            vec3 oc = center - r.origin();          //计算球心到射线起点的向量
            auto a = r.direction().length_squared();
            auto halfb = dot(oc, r.direction());
            auto c = oc.length_squared() - radius*radius;

            auto discriminant = halfb*halfb - a*c;                                         //计算判别式
            if(discriminant < 0 )
            {
                return false;                                                              //如果判别式小于0，说明射线与球没有交点
            }

            auto sqrtd = sqrt(discriminant);
            //找到最近的根
            auto root = (halfb - sqrtd) / a;
            if(root <= t_min || root >= t_max){                                             //判断根是否在范围内
                root = (halfb + sqrtd) / a;
                if(root <= t_min || root >= t_max){     
                    return false;                                                           //如果两个根都不在范围内，说明射线与球没有交点
                }
            }
            
            rec.t = root;
            rec.p = r.at(rec.t);
            rec.normal = (rec.p - center) / radius;                                 //计算法向量，球的法向量是从球心指向交点的向量，归一化后就是法向量
            return true;                                                                 //射线与球相交，返回true
        }

    private:
        point3 center;
        double radius;
};

#endif