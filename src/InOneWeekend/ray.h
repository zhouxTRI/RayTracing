#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
    public:
    ray() {}
    ray(const point3& origin, const vec3& direction)
        : orig(origin), dir(direction)
    {}

    const point3& origin() const { return orig; }  //返回射线的起点,不可变引用
    const vec3& direction() const { return dir; }  //返回射线的方向向量，不可变引用

    point3 at(double t) const {
        return orig + t*dir;  
    }

    private:
    point3 orig;
    vec3 dir;
};

#endif