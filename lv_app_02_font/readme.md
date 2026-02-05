# 🔠 Font

### 📖 项目简介

演示如何使用 LVGL 的 **标签 (lv_label)** 组件配合外部 **TTF 字库** 实现动态字体加载与管理。

### 🛠️ 移植使用

1. **拷贝文件**：将当前文件夹下的所有 `.c`、`.h` 文件以及 `font/` 文件夹拷贝到你的工程。
2. **配置头文件**：在你的项目中加入 `font_manager.h` 和 `font_conf.h`文件路径。
3. **资源配置**：确保 `font/` 目录下的 `.ttf` 文件路径在你的文件系统中可被正确访问。并在 `font_conf.h` 中配置自己的字库。

```c
#define FONT_PATH "./lv_app_02_font/font/" // 这里改成自己的字库路径

typedef enum 
{
    LV_FONT_TYPE_EN = 0,    ///< 英文字体
    LV_FONT_TYPE_CN         ///< 中文字体
} lv_font_type_t;            //这里可以视情况添加其他字库类型，如 LV_FONT_TYPE_NUMBER 等。

#define LV_FONT_TYPE_EN_PATH FONT_PATH "FiraCode-Regular-1.ttf" // 这里改为自己的字库文件

#define LV_FONT_TYPE_CN_PATH FONT_PATH "也字工厂小石头.ttf"     // 这里改为自己的字库文件


```

4. **调用** ：在主函数循环前包含头文件，并调用初始化以及应用函数。例如：

```c

#include<stdlib.h>
#include<stdio.h>
#include"lvgl.h"

extern void font_app_entry(void);

int main(void) 
{
   lv_init();
   // ... 在 lv_init() 后调用
   font_app_entry();
   // ...
}

```


### 🎬 视频演示

* **Bilibili** : [📺 点击跳转观看](https://www.bilibili.com/video/BV1ee6tBNEvk/?share_source=copy_web)
* **抖音** : [🎵 点击跳转观看](https://v.douyin.com/lIt2wB_pt90/)

---

[⬅️ 返回主目录](../README.md)
