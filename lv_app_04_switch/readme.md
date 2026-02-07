# 🔘 Useless Switch

### 📖 项目简介

这是一个基于 LVGL **开关 (lv_switch)** 和 **定时器 (lv_timer)** 实现的趣味交互应用。
灵感来源于 ***“Useless Box”***。

### 🛠️ 移植使用

1. **拷贝文件**：

   * 将当前文件夹下的源码拷贝到你的工程。
   * **重要**：必须将 `font/` (字体) 和 `audio/` (音频) 文件夹完整拷贝到项目资源目录，因为程序依赖这些外部资源。
2. **配置头文件**：在你的项目中包含调用处声明入口函数。
3. **参数配置**：你可以在 `switch_app.c` 顶部修改宏定义来调整 UI 布局与配色：

```c
/** 提示信息 UI 配置 */
#define MSG_FONT_SIZE       120         ///< 提示文字的字号大小
#define MSG_ALIGN_X_OFFSET  70          ///< 提示文字的 X 轴偏移量
#define MSG_COLOR_HEX       0x00FF00    ///< 提示文字颜色 (默认黑客绿)

/** 开关 UI 配置 */
#define SW_SIZE_W           300         ///< 开关控件的宽度
#define SW_SIZE_H           140         ///< 开关控件的高度
```

---

4. **调用**：在主函数初始化 LVGL 及驱动后，调用应用入口函数。

```c
#include "lvgl.h"

// 声明入口函数
extern void switch_app_entry(void);

int main(void) 
{
    lv_init();
    // ...

    // 启动无用开关应用
    // 内部会自动进行 font_init() 和 audio_init()
    switch_app_entry();

    // ...
}
```

### 🎬 视频演示

**Bilibili** : [📺 点击跳转观看](https://www.bilibili.com/video/BV1naFWzSEY3/?share_source=copy_web)

**抖音** : [🎵 点击跳转观看](https://v.douyin.com/oVkSjXo1Nlc/)

---

[⬅️ 返回主目录](../README.md)
