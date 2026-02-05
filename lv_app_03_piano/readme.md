# 🎹 Piano

### 📖 项目简介

基于 LVGL **按钮 (lv_btn)** 和 **事件处理 (event )** 机制实现的交互式模拟钢琴。

### 🛠️ 移植使用

1. **拷贝文件**：将当前文件夹下的 `piano.c` 和 `piano.h` 拷贝到你的工程。
2. **配置头文件**：在你的项目中包含 `piano.h` 文件路径。
3. **硬件与参数配置**：确保你的屏幕宽度足以容纳钢琴组件（建议横屏）。在 `piano.h` 中配置尺寸并对接你的初始化以及音频驱动。

```c
#define PIANO_SIZE_W        1424        ///< 钢琴控件的总宽度
#define PIANO_SIZE_H        280         ///< 钢琴控件的总高度

#define PIANO_OCTAVE_START  5           ///< 钢琴起始八度序号
#define PIANO_OCTAVE_COUNT  2           ///< 钢琴包含的八度数量


static inline void piano_app_init(void)
{
    // 在这里进行相关初始化配置
    // do sth...
}

static inline void piano_play_note_cmd(const char *note_name)
{
    // 在这里调用你的底层音频播放函数
    // your_audio_play(note_name); 
}
```

4.**调用** ：在主函数循环前包含头文件，并调用初始化以及应用函数。例如：

```c
#include<stdlib.h>
#include<stdio.h>
#include"lvgl.h"
#include"piano.h"

int main(void) 
{
    lv_init();

    // ... 在 lv_init() 后调用
    piano_app_init();
    piano_app_entry();

    // ...
}
```

### 🎬 视频演示

**Bilibili** : [📺 点击跳转观看](https://www.bilibili.com/video/BV1gaFez8EcS/?share_source=copy_web)

**抖音** : [🎵 点击跳转观看](https://v.douyin.com/uoIGZXClcvI/)

---

[⬅️ 返回主目录](../README.md)
