# 🧨 Happy Chinese New Year

### 📖 项目简介

基于 LVGL 实现的新年特效展示，集成 **解冻进度条** 、**图片缩放旋转动画等**。

### 🛠️ 移植使用

1. **拷贝文件** ：将 `HCNY_app.c` 以及 `res`、`util` 文件夹下的相关文件拷贝到你的工程目录。
2. **配置头文件** ：在你的代码中包含必要头文件，并确保资源路径 (`res_conf.h`) 已正确适配你的硬件。
3. **参数配置** ：在 `res/res_conf.h` 宏定义区域修改资源挂载路径、LVGL 虚拟盘符及文件名参数。

```c
/* ================== 环境路径配置 ================== */
#define IMAGE_PATH  "./lv_app_07_HCNY/res/"               ///< 图片资源路径
#define VEDIO_PATH  "./lv_app_07_HCNY/res/"               ///< 视频资源路径

/* ================== LVGL 虚拟文件系统配置 ================== */

/** 
 * @brief LVGL 注册的驱动器盘符前缀 (例如 A:) 
 */
#define DRIVER_LETTER  "A:"

/**
 * @brief 获取 LVGL 可识别的完整图片路径
 * @param name 图片文件名
 * @return 拼接了盘符的绝对路径字符串
 */
#define GET_IMAGE_PATH(name) (DRIVER_LETTER IMAGE_PATH name)

/* ================== 具体资源文件名 ================== */

#define VEDIO_NAME "gongxinifacai.mp4"  ///< 新年视频文件名
#define IMAGE_NAME "gongxinifacai.png"  ///< 恭喜发财图片文件名
```

4. **调用** ：在主函数循环前声明外部入口函数，并调用应用入口。

```c
extern void HCNY_app_entry(void);

int main(void)
{
    lv_init();
    // ... 屏幕与输入设备初始化 ...

    // 字库初始化
    lv_font_init();
    // 启动新年特效应用
    HCNY_app_entry();

    while(1) {
        lv_timer_handler();
        usleep(5000);
    }
}
```

### 🎬 视频演示

**Bilibili** : [📺 点击跳转观看](https://www.bilibili.com/video/BV1j4f3B6EBY/?share_source=copy_web)

**抖音** : [🎵 点击跳转观看](https://v.douyin.com/1uQcaGP2pu8/)

---

[⬅️ 返回主目录](../README.md)
