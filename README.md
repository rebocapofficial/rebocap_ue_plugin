# Rebocap Unreal Engine Plugin (rebocap-ue-plugin)

<div align="center">

![Rebocap Logo](Resources/Icon128.png)

### Official Unreal Engine (LiveLink) Plugin for Rebocap Motion Capture System
**官方 Rebocap 虚幻引擎 LiveLink 动捕插件（支持 UE 4.26 至 UE 5.8 全版本）**

[![Unreal Engine](https://img.shields.io/badge/Unreal%20Engine-4.26%20~%205.8-blue.svg?logo=unrealengine)](https://www.unrealengine.com/)
[![Release Version](https://img.shields.io/badge/Release-V2.0--beta07-brightgreen.svg)](https://github.com/rebocapofficial/rebocap-ue-plugin/releases)
[![License](https://img.shields.io/badge/License-MIT-orange.svg)](LICENSE)
[![Doc](https://img.shields.io/badge/Documentation-doc.hamer.xin-blueviolet)](http://doc.hamer.xin/)

[中文文档](#-中文说明) | [English](#-english)

</div>

---

## 📖 中文说明

**`rebocap-ue-plugin`** 是 Rebocap 动捕系统官方专为 **Epic Games Unreal Engine** 开发的高性能动捕插件。插件基于原生 WebSocket 通信协议与虚幻引擎 **LiveLink** 架构，实现零延迟、高精度的全身动作捕捉数据实时推流、骨骼自适应重定向与动画驱动。

### ✨ 核心特性

- 🚀 **全引擎版本兼容**：开箱即用支持 **Unreal Engine 4.26、4.27 以及 UE 5.0 至 5.8** 全部 11 个主流版本。
- 🎛️ **动捕控制中心（Dashboard）**：虚幻编辑器主工具栏内置可视化控制面板，支持一键连接、自定义端口、通信质量与数据状态实时监控。
- 🔄 **A2T 姿态自适应校准（A2T Node）**：内置智能姿态校准算法，支持 A-Pose 与 T-Pose 骨骼模型的实时空间对齐与重定向。
- ⚡ **高刷帧率插值（Frame Interpolation）**：亚帧级四元数球面线性插值（SLERP），消除动捕传输与高刷显示器渲染不同步带来的微小抖动。
- 🛡️ **Zero-Crash 运行环境容错加固**：纯动态非阻塞架构，即使宿主电脑缺失微软 VC++ 运行库也能顺畅进入工程，并提供保姆式一键修复与错误诊断指引。
- 🌐 **全链路国际化支持**：内置中/英/日多语言文案与悬停提示（Tooltip）。

---

### 📦 快速安装与使用

#### 1. 安装插件
1. 从 [Releases](https://github.com/rebocapofficial/rebocap-ue-plugin/releases) 下载对应虚幻引擎版本的预编译包（或直接下载源码）；
2. 解压到你的虚幻工程根目录下的 `Plugins/rebocap`（如无 `Plugins` 文件夹可手动新建）；
3. 打开工程，在 **编辑 -> 插件 (Plugins)** 中勾选并启用 **Rebocap** 插件（需同时启用 LiveLink）。

#### 2. 连接动捕
1. 打开 Rebocap 官方上位机软件并启动动捕服务；
2. 在虚幻编辑器顶部工具栏点击 **Rebocap 图标** 打开动捕控制中心；
3. 确认端口匹配并点击 **【连接】**，状态指示灯变为绿色即表示已连接。

#### 3. 动画蓝图接入
1. 打开角色的 **动画蓝图 (AnimBlueprint)**；
2. 在动画图表中右键搜索并添加 **`Rebocap 姿态` (Rebocap Body Pose)** 节点；
3. 连接输入姿态与输出姿态，并在节点细节面板中指定对应的 **骨骼映射资产 (Remap Asset)** 即可完成动捕驱动！

---

### 🛠️ 常见问题排查

- **提示缺少运行库 / 模块加载失败？**  
  请确保已安装微软官方 C++ 运行库合集：[Microsoft Visual C++ 2015-2022 Redistributable (x64)](https://aka.ms/vs/17/release/vc_redist.x64.exe)。

---

## 🌐 English

**`rebocap-ue-plugin`** is the official Unreal Engine motion capture plugin for **Rebocap**. Powered by Rebocap's WebSocket protocol and Unreal Engine's **LiveLink** framework, it provides ultra-low latency, high-precision real-time body tracking, bone retargeting, and animation driving.

### ✨ Key Features

- 🚀 **Full Engine Compatibility**: Supports Unreal Engine 4.26, 4.27, and UE 5.0 through 5.8 out of the box.
- 🎛️ **Control Center Dashboard**: Integrated editor toolbar panel for 1-click connection, port configuration, and live signal monitoring.
- 🔄 **A2T Adaptive Calibration Node**: Seamless real-time retargeting between A-Pose and T-Pose skeletal meshes.
- ⚡ **High-FPS SLERP Interpolation**: Sub-frame quaternion spherical linear interpolation eliminates jitter on high refresh rate displays.
- 🛡️ **Zero-Crash Resilience**: Pure dynamic non-blocking module loading architecture with built-in runtime environment diagnostics.
- 🌐 **Multi-Language Support**: Fully localized in English, Simplified Chinese, and Japanese.

---

### 📄 License & Support

- **License**: Released under the [MIT License](LICENSE).
- **Official Documentation**: [http://doc.hamer.xin/](http://doc.hamer.xin/)
- **Bug Reports & Feedback**: Please submit via [GitHub Issues](https://github.com/rebocapofficial/rebocap-ue-plugin/issues).
