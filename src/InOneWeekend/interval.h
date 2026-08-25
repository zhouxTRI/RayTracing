#ifndef INTERVAL_H
#define INTERVAL_H


class interval{
    public:
        double min,max;
        interval() : min(+ infinity), max(-infinity) {}         //默认构造函数，初始化为无效区间
        interval(double _min, double _max) : min(_min), max(_max) {}

        double size() const { return max - min; }                       //返回区间长度

        bool contains(double x) const { return min <= x && x <= max; }  //判断x是否在区间内

        bool surrounds(double x) const { return min < x && x < max; }   //判断x是否在区间内，严格小于

        double clamp(double x) const {                                  //将x限制在区间内，如果x小于区间最小值，返回最小值；如果x大于区间最大值，返回最大值；否则返回x
            if(x < min) return min;                                       //如果x小于区间最小值，返回最小值
            if(x > max) return max;                                       //如果x大于区间最大值，返回最大值
            return x;                                                     //否则返回x
        }

        static const interval empty,universe;  

};

const interval interval::empty  = interval(+infinity, -infinity);    //静态成员变量，表示空区间
const interval interval::universe = interval(-infinity, +infinity);  //静态成员变量，表示整个实数轴区间

#endif