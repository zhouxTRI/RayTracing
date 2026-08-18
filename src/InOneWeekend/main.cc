#include <iostream>

int main(){
    //image
    int image_width = 256;
    int image_height = 256;

    //render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for(int j=0; j < image_height; ++j){
        for(int i=0; i < image_width; ++i){
            int r = static_cast<int>(255.999 * double(i) / (image_width - 1)); 
            //255.999让最大值尽量接近255，避免出现256的情况（用static_cast<int>转换时会向下取整）
            int g = static_cast<int>(255.999 * double(j) / (image_height - 1));
            int b = 0;
            std::cout << r << ' ' << g << ' ' << b << '\n';
        }
    }
}