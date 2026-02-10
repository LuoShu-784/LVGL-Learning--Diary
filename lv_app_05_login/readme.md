# 🔐 Login

### 📖 项目简介

基于 LVGL 实现的登录界面，集成 **虚拟键盘** 、**密码验证**与 **音效反馈**。

### 🛠️ 移植使用

1. **拷贝文件** ：将 `login_app.c` 以及 `audio` 文件夹下的相关文件拷贝到你的工程目录。
2. **配置头文件** ：在你的代码中包含必要头文件，并确保音频驱动 (`audio_conf.h`) 已正确适配你的硬件。
3. **参数配置** ：在 `login_app.c` 头部宏定义区域修改默认密码、用户名及 UI 尺寸参数。

```c
/* --- 用户配置宏 (login_app.c) --- */

#define LOGIN_PASSWORD          "114514"        ///< 系统默认登录密码
#define LOGIN_USERNAME          "LuoShu"        ///< 显示的用户名
#define LOGIN_SUCCESS_AUDIO     "login_success" ///< 登录成功提示音 (不含后缀)
#define LOGIN_FAIL_AUDIO        "login_fail"    ///< 登录失败提示音 (不含后缀)

/* --- 布局尺寸配置 --- */
#define USER_PANEL_WIDTH        LV_PCT(35)      ///< 左侧面板宽度占比
#define AVATAR_SIZE             128             ///< 头像尺寸 (像素)
```

4. **音频接口适配** ：打开 `audio/audio_conf.h`，根据你的硬件平台实现音频初始化与播放函数。

```c
// audio/audio_conf.h

static inline void audio_init(void)
{
    // 在此处初始化你的音频硬件
}

static inline void audio_play(const char* url)
{
    // 在此处调用底层播放器
}
```

5. **调用** ：在主函数循环前调用应用入口函数即可。

```c
#include "login_app.h" // 假设你为 login_app.c 创建了头文件，或者使用 extern 声明

int main(void)
{
    lv_init();
    // ... 屏幕与输入设备初始化 ...

    // 启动登录应用
    login_app_entry();

    while(1) {
        lv_timer_handler();
        usleep(5000);
    }
}
```

### 🎬 视频演示

**Bilibili** : [📺 点击跳转观看](https://www.bilibili.com/video/BV1UHFDzgEQk/?share_source=copy_web)

**抖音** : [🎵 点击跳转观看](https://v.douyin.com/EyoWiSc6OZc)

---

[⬅️ 返回主目录](../README.md)
