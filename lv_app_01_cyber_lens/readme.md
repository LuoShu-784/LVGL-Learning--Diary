# 🔍 Cyber Lens

### 📖 项目简介

本项目利用 LVGL 的 **基础对象 (lv_obj)** 和 **样式设置 (styles)** 模拟出一种光学镜头的视觉效果。

### 🛠️ 移植使用

1. **拷贝文件**：将 `cyber_lens.c`、`lv_obj_cfg.c` 及 `lv_obj_cfg.h` 拷贝到你的工程。
2. **配置头文件**：在你的项目中加入 `lv_obj_cfg.h`文件路径。
3. **调用**：在 LVGL 初始化后调用：
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

### 🎬 视频演示

* **Bilibili** : [📺 点击跳转观看](https://www.bilibili.com/video/BV18m6nBGEQe/?share_source=copy_web)
* **抖音** : [🎵 点击跳转观看](https://v.douyin.com/Iwsk1lDTDe8/)

---

[⬅️ 返回主目录
](../README.md)
