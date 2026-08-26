#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>


// Usings

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double(){                          //返回[0,1)范围内的随机数
    return rand() / (RAND_MAX + 1.0);                   //因为rand()返回的最小值是0，最大值是RAND_MAX，除以RAND_MAX+1.0可以确保结果小于1
}
inline double random_double(double min, double max){
    return min + (max - min) * random_double();         //返回[min,max)范围内的随机数
}

// Common Headers

#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif