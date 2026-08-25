#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
    public:
        sphere(const point3& center, double radius)
        : center(center), radius(std::fmax(0.0, radius)) {}

        bool hit(const ray&r, interval ray_t, hit_record& rec) const override {
            vec3 oc = center - r.origin();                                                  //计算球心到射线起点的向量
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
            if(!ray_t.surrounds(root)){                                                     //判断根是否在范围内
                root = (halfb + sqrtd) / a;                                                 //如果不在范围内，尝试另一个根
                if(!ray_t.surrounds(root)){
                    return false;                                                           //如果两个根都不在范围内，说明射线与球没有交点
                }
            }
            
            rec.t = root;                                                           //根作为射线参数t，可用来计算交点和法向量
            rec.p = r.at(rec.t);                                                    //计算交点，P(t) = A + t*B，其中A是射线的起点，B是射线的方向向量
            vec3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);
           
            return true;                                                                 //射线与球相交，返回true
        }

    private:
        point3 center;
        double radius;
};

#endif