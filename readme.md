# 📺 LVGL 学习日记

![LVGL](https://img.shields.io/badge/LVGL-v8.3-orange?style=flat-square&logo=gitbook)
![Language](https://img.shields.io/badge/Language-C-blue?style=flat-square&logo=c)
![Platform](https://img.shields.io/badge/Platform-Simulator%20%7C%20Embedded-green?style=flat-square)
![Bilibili](https://img.shields.io/badge/Bilibili-洛书--LS-fb7299?style=flat-square&logo=bilibili)
![Douyin](https://img.shields.io/badge/Douyin-洛书:（嵌入式进阶ing）-000000?style=flat-square&logo=tiktok)

欢迎来到我的 LVGL 学习日记！🚀

这个仓库是每期 ***《LVGL 学习日记》*** 视频配套代码的 **存档点**。

我把视频里那些不成熟的 UI 创意整理分享出来，并没有什么高深的技术，更多是希望给同样在学习的小伙伴提供一点灵感和参考。如果你也正在学习 LVGL，欢迎一起交流探讨!

---

## 🚀 项目索引

这里记录了每期视频对应的实现代码。你可以根据需要跳转到对应的平台观看演示。

| 编号 |              项目名称              |      实现功能      |               核心依据               |                                                        传送门                                                        |
| :--: | :--------------------------------: | :----------------: | :----------------------------------: | :-------------------------------------------------------------------------------------------------------------------: |
|  01  | [Cyber Lens](./lv_app_01_cyber_lens/) |  光学镜头视觉效果  | 基础对象 (obj) 与 样式设置 (styles) | [📺 B站](https://www.bilibili.com/video/BV18m6nBGEQe/?share_source=copy_web) / [🎵 抖音](https://v.douyin.com/Iwsk1lDTDe8/) |
|  02  |       [Font](./lv_app_02_font/)       | 字体加载与动态管理 |     标签 (label) 与 字库 (font )     | [📺 B站](https://www.bilibili.com/video/BV1ee6tBNEvk/?share_source=copy_web) / [🎵 抖音](https://v.douyin.com/lIt2wB_pt90/) |
|  03  |      [Piano](./lv_app_03_piano/)      |   交互式模拟钢琴   |   按钮 (btn) 与 事件处理 (event )   | [📺 B站](https://www.bilibili.com/video/BV1gaFez8EcS/?share_source=copy_web) / [🎵 抖音](https://v.douyin.com/uoIGZXClcvI/) |
|  04  |     [Switch](./lv_app_04_switch/)     |  趣味“无用开关”  |   开关 (switch) 与 定时器 (timer)   | [📺 B站](https://www.bilibili.com/video/BV1naFWzSEY3/?share_source=copy_web) / [🎵 抖音](https://v.douyin.com/oVkSjXo1Nlc/) |
|  05  |      [Login](./lv_app_05_login)      |      登录页面      | 文本框 (textarea) 与键盘 (keyboard) | [📺 B站](https://www.bilibili.com/video/BV1UHFDzgEQk/?share_source=copy_web) / [🎵 抖音](https://v.douyin.com/EyoWiSc6OZc) |
|  06  |  [Day Night](./lv_app_06_day_night)  |    昼夜切换按钮    |             动画（anim）             | [📺 B站](https://www.bilibili.com/video/BV1ijZJBxE3o/?share_source=copy_web) / [🎵 抖音](https://v.douyin.com/qA4ffy18gQE/) |

---

## 🛠️ 快速上手

1. **克隆仓库** ：将本仓库克隆到本地。
2. **集成代码** ：将对应的项目文件夹拷贝到你的工程目录。
3. **调用** ：在主函数循环前包含头文件，并调用初始化以及应用函数。例如：

```c
#include <stdlib.h>
#include <stdio.h>
#include "lvgl.h"

extern void lv_app_entry(void);

int main(void) 
{
   lv_init();
   // ... 在 lv_init() 后调用
   lv_app_entry();
   // ...
}
```

---

## 💬 交流分享

**关注我的动态：**

* **Bilibili** : [@洛书-LS](https://b23.tv/14FemDt)
* **抖音** : [@洛书：（嵌入式进阶ing）](https://v.douyin.com/kNiE-n5Vjcs)

**反馈建议：** 如果你在运行代码时遇到坑，或者有更好的实现想法，欢迎提 **Issue** 或在评论区滴滴我。

---

## 📜 许可证

本项目采用 [MIT License](https://www.google.com/search?q=LICENSE) 开源。
