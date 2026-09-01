# Ray Tracing 光线追踪器

一个使用C++实现的高性能光线追踪渲染引擎，基于Peter Shirley的《Ray Tracing in One Weekend》教程。该项目演示了现代计算机图形学中光线追踪算法的核心原理和实现技巧。仅自学使用。

## 🎯 项目概述

本项目实现了一个完整的光线追踪渲染系统，能够生成逼真的3D场景图像。通过模拟光线与物体的相互作用，包括反射、折射和漫反射，生成具有真实感的计算机图形。

**主要特点：**
- ✨ 支持多种材质效果（漫反射、镜面反射、折射）
- 🎬 逼真的景深效果（Depth of Field）
- 📸 多重采样反锯齿（MSAA）
- 🎨 伽马校正和色彩管理
- ⚡ 递归光线追踪引擎
- 📊 PPM格式图像输出

## 📋 功能特性

### 核心渲染功能

#### 1. **光线追踪算法**
- 递归光线追踪引擎，模拟光线在场景中的传播路径
- 支持自定义最大递归深度，防止无限递归
- 高效的光线-物体碰撞检测算法

#### 2. **物体表示**
- **球体（Sphere）**：完整的光线-球体相交计算
  - 使用判别式方法求解二次方程
  - 精确的表面法向量计算
  - 支持材质绑定

#### 3. **材质系统**

**Lambertian（兰伯特式漫反射）**
- 实现漫反射材质，模拟粗糙表面
- 半球形随机光线散射方向
- 可自定义反射率（Albedo）
- 适用于非金属物体（布料、木材、混凝土等）

**Metal（金属镜面反射）**
- 完美镜面反射和粗糙金属表面
- 可调节的表面粗糙度（Fuzz）参数
- 控制反射光线的散射程度
- 真实模拟金属材质外观

**Dielectric（介质折射）**
- 精确的Snell折射定律实现
- Schlick's approximation 菲涅尔反射近似
- 全内反射（Total Internal Reflection）处理
- 支持光线在介质间传播
- 逼真的玻璃、水、钻石等透明材质

#### 4. **相机系统**
- **视场角（vfov）**：控制相机的视野范围
- **观察方向**：支持自定义相机位置和观察点
- **景深效果（Defocus）**：
  - 焦点距离控制
  - 失焦角度（defocus_angle）调节
  - 逼真的光学模糊效果
- **多重采样反锯齿**：每像素多次采样，提高图像质量

#### 5. **图像处理**
- **Gamma校正**：线性空间到感知空间的转换
- **颜色空间转换**：正确的色彩管理流程
- **采样平均**：多重采样结果加权平均
- **范围夹持**：防止颜色溢出

## 🏗️ 项目结构

```
RayTracing/
├── CMakeLists.txt              # CMake构建配置文件
├── src/
│   ├── external/
│   │   └── stb_image.h          # 图像处理库
│   └── InOneWeekend/            # 主要实现目录
│       ├── main.cc              # 程序入口，场景配置
│       ├── camera.h             # 相机类和渲染循环
│       ├── ray.h                # 光线类定义
│       ├── vec3.h               # 三维向量类
│       ├── color.h              # 颜色处理和Gamma校正
│       ├── hittable.h           # 可碰撞物体基类
│       ├── hittable_list.h      # 物体列表容器
│       ├── sphere.h             # 球体类实现
│       ├── material.h           # 材质系统（漫反射、金属、介质）
│       ├── interval.h           # 区间类（用于光线参数范围）
│       └── rtweekend.h          # 工具函数和常量定义
├── build/                       # 构建输出目录
├── *.ppm                        # 输出的图像文件（PPM格式）
└── README.md                    # 项目文档

```

## 🚀 编译和运行

### 系统要求
- C++14 或更高版本
- CMake 3.10+
- 支持的编译器：GCC、Clang、MSVC

### 编译步骤

```bash
# 创建构建目录
mkdir build
cd build

# 使用CMake生成构建文件
cmake ..

# 编译项目
cmake --build .
```

### 运行程序

```bash
# 进入构建目录
cd build

# 运行编译后的可执行文件，输出重定向到PPM文件
./inOneWeekend > output.ppm

# 或者（Windows）
inOneWeekend.exe > output.ppm
```

### 查看输出图像

生成的PPM文件可以使用任何支持PPM格式的图像查看器打开：
- Windows：IrfanView、FastStone等
- Linux：GIMP、ImageMagick等
- macOS：Preview、GIMP等

## ⚙️ 参数配置

在 [main.cc](src/InOneWeekend/main.cc) 中可以配置以下参数来调整渲染效果：

### 相机参数
```cpp
cam.aspect_ratio = 16.0 / 9.0;      // 图像宽高比（默认16:9）
cam.image_width = 1200;              // 输出图像宽度（像素）
cam.samples_per_pixel = 10;          // 每像素采样数（越高质量越好，速度越慢）
cam.max_depth = 50;                  // 光线递归最大深度

cam.vfov = 20;                       // 垂直视场角（度数）
cam.lookfrom = point3(13, 2, 3);    // 相机位置
cam.lookat = point3(0, 0, 0);       // 观察目标点
cam.vup = vec3(0, 1, 0);            // 相机上方向

cam.defocus_angle = 0.6;            // 景深效果强度
cam.focus_dist = 10;                // 焦点距离
```

### 场景配置

可以通过修改main.cc中的场景生成代码来改变场景内容：

```cpp
// 添加材质
auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
auto material_metal = make_shared<metal>(color(0.8, 0.6, 0.2), 0.3);
auto material_glass = make_shared<dielectric>(1.5);

// 添加物体到场景
world.add(make_shared<sphere>(point3(0, 0, 0), 1.0, material_ground));
```

## 📊 示例场景

项目包含一个随机生成的三球场景加随机小球的演示，展示了：

1. **地面球体**：大型兰伯特漫反射球体，作为场景基础
2. **三个主球体**：
   - 左球：玻璃介质球体，展示折射效果
   - 中球：棕色漫反射球体
   - 右球：铜色金属球体，展示镜面反射
3. **随机小球**：
   - 80% 概率：随机颜色的漫反射球体
   - 15% 概率：随机颜色的金属球体，具有随机粗糙度
   - 5% 概率：玻璃球体，展示折射

## 🔮 未来可实现的技术

### 1. **高级几何体**
- [ ] **三角形网格（Triangle Mesh）**：支持复杂的3D模型导入
  - OBJ/FBX文件加载
  - 自动法向量计算
  - 顶点法向量插值
- [ ] **立方体（Cuboid）**：参数化立方体原始体
- [ ] **平面（Plane）**：无限平面和有限平面
- [ ] **圆柱体、圆锥体、椭球体**：更多基本几何形状
- [ ] **Bezier曲面和NURBS**：高级曲面表示

### 2. **加速结构**
- [ ] **包围盒层级结构（BVH - Bounding Volume Hierarchy）**
  - 树形结构加速光线与物体的相交测试
  - 显著提升大场景的渲染速度
  - 支持动态BVH更新
- [ ] **K-D树**：空间分割数据结构
- [ ] **Octree**：八叉树加速结构
- [ ] **网格加速**：均匀网格划分

### 3. **光源系统**
- [ ] **面积光（Area Lights）**
  - 矩形光源
  - 圆形光源
  - 自定义形状光源
- [ ] **点光源（Point Lights）**：无衰减和有衰减选项
- [ ] **聚光灯（Spot Lights）**：方向性光源
- [ ] **环境光贴图（HDRI）**：使用真实世界HDR图像作为环境光
- [ ] **发光材质（Emissive Materials）**：自发光表面

### 4. **材质系统扩展**
- [ ] **微表面模型（Microfacet BRDF）**
  - Fresnel项精确计算
  - 几何项和分布项
  - Cook-Torrance模型
- [ ] **各向异性反射（Anisotropic Reflection）**
- [ ] **次表面散射（Subsurface Scattering）**：皮肤、石蜡等材质
- [ ] **布料材质**：特殊的漫反射模型
- [ ] **纹理映射**
  - 漫反射贴图
  - 法线贴图
  - 粗糙度贴图
  - 高度贴图
- [ ] **过程纹理**：Perlin噪声、Worley噪声等
- [ ] **PBR工作流**：基于物理的渲染完整流程

### 5. **渲染算法优化**
- [ ] **路径追踪（Path Tracing）**：更精确的光线传输模拟
- [ ] **双向路径追踪（Bidirectional Path Tracing）**
- [ ] **Metropolis Light Transport**：自适应采样
- [ ] **光子映射（Photon Mapping）**：处理焦散效果
- [ ] **重要性采样（Importance Sampling）**
  - 光源采样
  - 半球采样优化
  - 环境图采样
- [ ] **降噪（Denoising）**
  - AI降噪算法集成
  - 时间降噪
  - 空间降噪

### 6. **相机系统增强**
- [ ] **运动模糊（Motion Blur）**：动态物体轨迹效果
- [ ] **光学畸变**：真实相机的几何畸变
- [ ] **色差**：色分离效果
- [ ] **景深的高级效果**：自定义光圈形状
- [ ] **焦点堆栈（Focus Stacking）**

### 7. **性能优化**
- [ ] **多线程并行渲染**：CPU多核利用
- [ ] **GPU加速**：CUDA/OptiX支持NVIDIA GPU
- [ ] **OpenCL支持**：跨平台GPU计算
- [ ] **增量渲染**：实时反馈和渐进式渲染
- [ ] **自适应采样**：根据场景复杂度动态调整采样率

### 8. **场景管理**
- [ ] **场景编辑器GUI**：实时编辑界面
- [ ] **模型导入支持**
  - Wavefront OBJ
  - FBX格式
  - glTF 2.0
  - USDZ等
- [ ] **场景序列化**：保存和加载场景
- [ ] **动画系统**
  - 关键帧动画
  - 物体变换动画
  - 相机轨迹动画
- [ ] **层系统**：分层物体管理

### 9. **后期处理**
- [ ] **色调映射（Tone Mapping）**
  - Reinhard操作符
  - 胶片模拟
  - ACES工作流
- [ ] **色彩分级**
  - LUT应用
  - HSL调整
  - 选择性色彩
- [ ] **景深后处理**：高级焦点模糊
- [ ] **运动矢量模糊**

### 10. **高级视觉效果**
- [ ] **体积雾（Volumetric Fog）**：体积光散射
- [ ] **焦散（Caustics）**：水下光线折射效果
- [ ] **雨、雪、灰尘粒子系统**
- [ ] **大气散射**：Rayleigh和Mie散射
- [ ] **极光（Aurora）**：环境光学现象

### 11. **实验性功能**
- [ ] **机器学习集成**：神经网络去噪和超分辨率
- [ ] **实时预览**：交互式渲染窗口
- [ ] **虚拟现实（VR）支持**：360°图像输出
- [ ] **光线追踪调试器**：可视化光线路径
- [ ] **渲染时间估计**：智能采样调度

### 12. **用户界面和工具链**
- [ ] **命令行接口（CLI）改进**
  - 配置文件支持（YAML/JSON）
  - 参数预设
  - 批量渲染
- [ ] **可视化调试工具**
  - 法向量可视化
  - 深度图预览
  - 着色模式切换
- [ ] **性能分析**：渲染时间分析和统计
- [ ] **插件系统**：可扩展的材质和效果

## 📈 性能指标

- **单次渲染**：1200×675分辨率，10采样率约需几秒到几十秒（取决于CPU）
- **采样率影响**：采样率越高，图像质量越好，但渲染时间成线性增加
- **递归深度影响**：深度越大，反射折射效果越逼真，但性能消耗越大

## 🔧 常见问题与解决方案

### 问题1：法向量着色显示异常
**现象**：图像着色不正确或显示全黑

**原因**：在 `src/InOneWeekend/main.cc` 中，`hit_sphere()` 函数返回类型不正确

**解决方案**：
- `hit_sphere()` 必须返回 `double` 类型的 `t` 值（光线参数），而不是 `bool` 类型
- 返回值 `t` 用于后续的法向量计算和着色

```cpp
// ❌ 错误做法
bool hit_sphere(const point3& center, double radius, const ray& r) {
    // ...
}

// ✅ 正确做法
double hit_sphere(const point3& center, double radius, const ray& r) {
    // 返回t值用于着色
}
```

### 问题2：光线-球体相交计算精度问题
**现象**：渲染结果有黑点、闪烁或光线穿过物体

**原因**：使用标准的二次方程求解法导致数值稳定性问题

**解决方案**：
使用 **half_b 优化形式**来求解光线-球体相交，这是标准的数值计算技巧：

```cpp
// 标准形式：at² + bt + c = 0
// 优化为：at² + (2h)t + c = 0，其中 b = 2h
// 这样可以减少浮点运算误差并提高数值稳定性

double a = dot(r.direction(), r.direction());
double half_b = dot(oc, r.direction());  // oc = ray.origin - center
double c = dot(oc, oc) - radius * radius;

double discriminant = half_b * half_b - a * c;
if (discriminant < 0) return -1.0;  // 无交点

double sqrt_discriminant = sqrt(discriminant);
double t = (-half_b - sqrt_discriminant) / a;  // 取较小的t值（最近的交点）
return t;
```

**为什么这样做**：
- 减少浮点运算错误
- 避免数值溢出
- 提高渲染结果的稳定性和准确性

## 🎓 学习资源

本项目基于以下优秀教程和资源：
- Peter Shirley - *Ray Tracing in One Weekend*
- Inigo Quilez - 距离场和光线追踪资源
- Shadertoy - 实时光线追踪示例
- *Physically Based Rendering* 系列书籍

## 📝 代码示例

### 创建简单场景

```cpp
// 创建物体列表
hittable_list world;

// 创建材质
auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
auto metal_material = make_shared<metal>(color(0.8, 0.6, 0.2), 0.3);
auto glass_material = make_shared<dielectric>(1.5);

// 添加物体
world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));  // 地面
world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, glass_material));        // 玻璃球
world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, ground_material));      // 漫反射球
world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, metal_material));        // 金属球

// 配置相机并渲染
camera cam;
cam.aspect_ratio = 16.0 / 9.0;
cam.image_width = 1200;
cam.samples_per_pixel = 100;
cam.max_depth = 50;
cam.render(world);
```

## 📄 许可证

本项目基于 Peter Shirley 的 *Ray Tracing in One Weekend* 教程实现。

## 🤝 贡献

欢迎提交问题报告、功能建议和代码改进！

## 📧 联系方式

如有问题或建议，欢迎通过项目Issue提出。

---

**最后更新**：2026年9月  
**当前版本**：0.1.0（基础光线追踪实现）
