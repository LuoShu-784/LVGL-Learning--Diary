# 🌓 Day Night Switch

### 📖 项目简介

基于 LVGL 实现的 **纯代码绘图** 昼夜切换开关。集成 **视差滚动背景** 、**日/月过渡动画** 与 **多层云朵/星空** 效果。

### 🛠️ 移植使用

1. **拷贝文件** ：将 `day_night_app.c` 以及 `ui`、`util` 文件夹下的相关文件拷贝到你的工程目录。
2. **配置头文件** ：在你的代码中包含必要头文件。
3. **参数配置** ：在 `ui/day_night_ui_conf.h` 中修改 UI 元素的尺寸、颜色及动画参数。

```c
/* --- 整体背景色 --- */
#define BG_NIGHT_COLOR     0x424242

/* --- 按钮主体配置 --- */
#define BTN_SIZE            150     ///< 按钮直径
#define BTN_X_OFS           (-150)  ///< 按钮初始 X 偏移
#define BTN_Y_OFS           0       ///< 按钮初始 Y 偏移

...

/* --- 云朵配置 --- */
#define CLOUD_BG_COLOR      0xa3c2e0
#define CLOUD_FG_COLOR      0xffffff
#define CLOUD_COUNT         12

/* --- 星星配置 --- */
#define STAR_COUNT      6
#define STAR_COLOR      0xffffff
```

4. **调用** ：在主函数循环前声明外部入口函数，并调用应用入口。

```c
// 声明外部入口函数
extern void day_night_app_entry(void);

int main(void)
{
    lv_init();
    // ... 屏幕与输入设备初始化 ...

    // 启动昼夜切换应用
    day_night_app_entry();

    while(1) {
        lv_timer_handler();
        usleep(5000);
    }
}
```

### 🎬 视频演示

**Bilibili** : [📺 点击跳转观看](https://www.bilibili.com/video/BV1ijZJBxE3o/?share_source=copy_web)

**抖音** : [🎵 点击跳转观看](https://v.douyin.com/qA4ffy18gQE/)

---

[⬅️ 返回主目录](../README.md)
