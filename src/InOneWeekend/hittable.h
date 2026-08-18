#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

class hit_record {
    public:
        point3 p;  //交点
        vec3 normal;  //法向量
        double t;  //射线参数t
        bool front_face;  //是否是正面交点

        void set_face_normal(const ray& r, const vec3& outward_normal){
            //设置法向量的方向，使其始终指向射线的外侧，确保法向量的方向与射线方向相反
            //参数outward_normal，单位长度
            front_face = dot(r.direction(), outward_normal) < 0;  //如果射线方向与法向量的点积小于0，说明射线方向与法向量的方向相反，即射线从物体外部射入，交点是正面交点
            normal = front_face ? outward_normal : -outward_normal;  //如果是正面交点，法向量保持不变，否则取反
        }
};
class hittable {
    public:
        virtual ~hittable() = default;  //虚析构函数，确保派生类对象被正确销毁

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;  //纯虚函数，判断射线是否与物体相交;=0,表示该函数没有实现，必须在派生类中实现
};

#endif