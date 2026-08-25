#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable {
    public:
        std::vector<shared_ptr<hittable>> objects;  //存储hittable对象的智能指针的向量
        hittable_list() {}
        hittable_list(shared_ptr<hittable> object) { add(object); }  //构造函数，添加一个hittable对象

        void clear() { objects.clear(); }  
        void add(shared_ptr<hittable> object) { objects.push_back(object);}
        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            hit_record temp_rec;
            bool hit_anything = false;
            auto closest_so_far = ray_t.max;                                            //初始化最近的交点距离为ray_t的最大值，确保第一次找到的交点一定是最近的交点

            for( const auto& object : objects)
            {                                                                           //调用的是object的hit函数，在main程序里会传入sphere对象的智能指针，调用的是sphere类的hit函数
                if(object->hit(r, interval(ray_t.min, closest_so_far), temp_rec))                  //当object的t大于closest_so_far时，hit函数返回false，说明该交点不是最近的交点，忽略该交点
                {
                    hit_anything = true;
                    closest_so_far = temp_rec.t;                                         //更新最近的交点距离
                    rec = temp_rec;                                                      //更新交点信息
                }
            }

            return hit_anything;
        }

};

#endif