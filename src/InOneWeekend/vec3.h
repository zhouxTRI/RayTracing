#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

using std::sqrt;

class vec3 {
    public:
        double e[3];  //存储向量的三个分量
        vec3() : e{0, 0, 0} {}      //默认构造函数
        vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}  //构造函数

        double x() const { return e[0]; }  //返回x分量
        double y() const { return e[1]; }  //返回y分量
        double z() const { return e[2]; }  //返回z分量

        //重载运算符，语法糖，方便向量的加减乘除等操作
        vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }  //重载一元运算符-,返回一个新的vec3对象，其分量为原向量的相反数
        double operator[](int i) const { return e[i]; }  //重载下标运算符[]，返回对应分量
        double& operator[](int i) { return e[i]; }  //重载下标运算符[]，返回对应分量的引用

        vec3& operator+=(const vec3 &v) {  //重载+=运算符，实现向量的加法赋值
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;
        }
        vec3& operator*=(const double t) {  //重载*=运算符，实现向量的数乘赋值
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }
        vec3& operator/=(const double t) {  //重载/=运算符，实现向量的数除赋值
            return *this *= 1/t;        //用乘法+除法，代替除法，减少全除法运算的开销
        }
        
        double length() const{
            return sqrt(length_squared());  //返回向量的长度
        }
        double length_squared() const{
            return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];  //返回向量的长度的平方
        }

        bool near_zero() const {                                              //向量接近零向量，返回true
            auto s = 1e-8;                                                    //定义一个很小的数，作为判断向量是否接近零向量的阈值
            return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);  //如果向量的三个分量都小于阈值，则认为向量接近零向量;fabs是取绝对值函数，防止分量为负数
        }

        //生成随机向量，长度在[0,1)之间，方向随机
        static vec3 random(){    
            return vec3(random_double(), random_double(), random_double());                         //static在这里的作用是将函数与类关联，而不是与类的对象关联。
        }                                                                                           //也就是说，static成员函数可以在没有创建类的对象的情况下被调用。
        static vec3 random(double min, double max){                                                 //它们不能访问类的非静态成员变量或非静态成员函数，因为它们没有this指针。
            return vec3(random_double(min,max), random_double(min,max), random_double(min,max));    //静态成员函数通常用于实现与类相关的功能，但不依赖于类的具体实例。
        }
};




//point3 
using point3 = vec3;   //point3是vec3的别名，表示三维空间中的点

//向量实用函数,用inline修饰，编译器会将函数体直接嵌入到调用处，减少函数调用的开销
inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {         //重载<<运算符，实现向量的输出,返回ostream对象的引用
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];                 //输出向量的三个分量
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {                       //重载+运算符，实现向量的加法
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);         //返回一个新的vec3对象，其分量为两个向量对应分量的和
}
inline vec3 operator-(const vec3 &u, const vec3 &v) {                       //重载-运算符，实现向量的减法
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);         //返回一个新的vec3对象，其分量为两个向量对应分量的差
}
inline vec3 operator*(const vec3 &u, const vec3 &v) {                       //重载*运算符，实现向量的点乘：用于计算两个向量的夹角余弦值，或者判断两个向量是否垂直
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);         //返回一个新的vec3对象，其分量为两个向量对应分量的积
}
inline vec3 operator*(double t, const vec3 &v) {                            //重载*运算符，实现向量的数乘：用于将向量缩放t倍
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);                              //返回一个新的vec3对象，其分量为向量的分量乘以标量t
}
inline vec3 operator*(const vec3 &v, double t) {                            //重载*运算符，实现向量的数乘
    return t * v;                                                           //调用上面的重载函数
}
inline vec3 operator/(vec3 v, double t) {                                   //重载/运算符，实现向量的数除
    return (1/t) * v;                                                       //调用上面的重载函数
}
inline double dot(const vec3 &u, const vec3 &v) {                           //重载dot函数，实现向量的点积:用于计算两个向量的夹角余弦值，或者判断两个向量是否垂直
    return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];             //返回两个向量的点积，为标量;小于0，说明两个向量夹角大于90度;等于0，说明两个向量垂直;大于0，说明两个向量夹角小于90度
}
inline vec3 cross(const vec3 &u, const vec3 &v) {                           //重载cross函数，实现向量的叉积：用于计算两个向量的法向量，或者判断两个向量是否平行
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);                         //返回两个向量的叉积
}
inline vec3 unit_vector(const vec3& v) {                                           //重载unit_vector函数，实现向量的单位化
    return v / v.length();                                                  //返回一个新的vec3对象，其分量为原向量的分量除以向量的长度
}
inline vec3 random_in_unit_disk() {                                         //返回一个在单位圆盘内随机采样的点，范围为[-1,1) * [-1,1)
    while(true){
        auto p = vec3(random_double(-1,1), random_double(-1,1), 0);
        if(p.length_squared() < 1) return p;                               //如果点在单位圆盘内，则返回该点
    }

}

//用拒绝方法生成单位球内的随机向量
inline vec3 random_in_unit_sphere() {
    while(true){
        auto p = vec3::random(-1,1);
        if(p.length_squared() < 1) return p;
    }
}
//将单位球内的随机向量归一化，得到随机单位球向量（从单位球心指向球面上的随机点的向量）
inline vec3 random_unit_vector(){
    return unit_vector(random_in_unit_sphere());
}
//确定随机向量在指定法向量的半球内
inline vec3 random_on_hemisphere(const vec3& normal) {
    vec3 on_unit_sphere = random_unit_vector();
    if(dot(on_unit_sphere, normal) > 0.0)   
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}
//根据入射向量和单位法向量求反射向量
inline vec3 reflect(const vec3& v, const vec3& n) {     //反射向量
    return v + (2 * -dot(v,n) * n);                     //v是入射向量，n是单位法向量，返回反射向量
}
//根据入射向量和单位法向量求折射向量
inline vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat){
    auto cos_theta = fmin(dot(-uv, n) , 1.0);                                   //计算入射角的余弦值，fmin是返回两个参数中较小的一个，为了避免浮点数误差导致的cos_theta大于1
    vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);                    //计算折射向量的垂直分量
    vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;   //计算折射向量的平行分量,fabs返回绝对值，避免浮点数误差导致的负数开方
    return r_out_perp + r_out_parallel;  //返回折射向量 
}

#endif
