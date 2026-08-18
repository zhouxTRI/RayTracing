#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

class hit_record {
    public:
        point3 p;  //交点
        vec3 normal;  //法向量
        double t;  //射线参数t
};
class hittable {
    public:
        virtual ~hittable() = default;  //虚析构函数，确保派生类对象被正确销毁

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;  //纯虚函数，判断射线是否与物体相交;=0,表示该函数没有实现，必须在派生类中实现
};

#endif