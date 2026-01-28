# Nuitka Shim Launcher

[![Build Launcher](https://github.com/jamosnet/Nuitka-Shim-Launcher/actions/workflows/build.yml/badge.svg)](https://github.com/jamosnet/Nuitka-Shim-Launcher/actions/workflows/build.yml)
![Platform](https://img.shields.io/badge/platform-Windows-blue)
![Language](https://img.shields.io/badge/language-C%2B%2B-orange)
![License](https://img.shields.io/badge/license-MIT-green)

[中文说明](#chinese) | [English Description](#english)

---

<a name="english"></a>
## 🇬🇧 English Description

### What is this?
**Nuitka Shim Launcher** is a lightweight (approx. 50KB), dependency-free C++ launcher designed for Python applications packaged with [Nuitka](https://nuitka.net/).

It allows you to achieve a clean directory structure similar to PyInstaller's `onedir` mode (with an `_internal` folder), keeping your root directory free of DLLs and pyd files, while maintaining the instant startup speed of Nuitka's standalone mode.

### Why do I need it?
*   **Nuitka `--standalone`**: Fast startup, but the root directory is cluttered with `python.dll`, `vcruntime.dll`, and other files.
*   **Nuitka `--onefile`**: Clean single file, but slow startup due to temporary decompression.
*   **Nuitka Shim Launcher**: **The Best of Both Worlds.** It keeps the root clean (only one EXE) and hides dependencies in `_internal`, with **zero startup latency**.

### ✨ Features
*   **Dynamic Mapping**: No configuration needed. If you rename the launcher to `MyApp.exe`, it automatically launches `_internal\MyApp.exe`.
*   **Ultra Lightweight**: Written in pure C++ (WinAPI), ~50KB in size.
*   **No Dependencies**: No MSVC runtime required on the client machine.
*   **Unicode Support**: Fully supports paths with Chinese or other special characters.

### 🚀 Usage

#### 1. Download the Launcher
Go to the [Actions](../../actions) page, find the latest successful run, and download the `Shim.exe` artifact.

#### 2. Prepare your Nuitka Build
Compile your Python project using Nuitka's standalone mode:
```bash
python -m nuitka --standalone --output-dir=dist myscript.py
```

#### 3. Organize the Directory
1.  Rename your Nuitka output folder (e.g., `myscript.dist`) to **`_internal`**.
2.  Place the downloaded `Shim.exe` next to the `_internal` folder.
3.  **Rename `Shim.exe`** to match the name of the executable inside `_internal`.

**Example:**
If your Nuitka output is `_internal\monitor.exe`, rename `Shim.exe` to `monitor.exe`.

#### 📁 Directory Structure Comparison

**Before (Messy):**
```text
MyProgram/
├── myscript.exe
├── python310.dll
├── libcrypto.dll
├── vcruntime140.dll
├── ... (dozens of files)
```

**After (Clean):**
```text
MyProgram/
├── myscript.exe         <-- This is the Shim (Renamed)
└── _internal/           <-- Dependencies hidden here
    ├── myscript.exe     <-- Real Nuitka App
    ├── python310.dll
    └── ...
```

---

<a name="chinese"></a>
## 🇨🇳 中文说明

### 这是什么？
**Nuitka Shim Launcher** 是一个专为 [Nuitka](https://nuitka.net/) 打包应用设计的轻量级（约 50KB）C++ 启动器垫片。

它可以让你实现类似 PyInstaller `_internal` 模式的目录结构，将杂乱的 DLL 和依赖文件隐藏在子目录中，还你一个干净的软件根目录，同时保留 Nuitka 独立模式的**秒开**速度。

### 为什么需要它？
*   **Nuitka `--standalone`**: 启动快，但根目录会有一大堆 `python.dll`、`pyd` 文件，很难看。
*   **Nuitka `--onefile`**: 单文件很干净，但每次启动都需要解压到临时目录，大文件启动很慢。
*   **Nuitka Shim Launcher**: **两全其美**。既保持目录干净（只有一个 EXE），又把依赖藏在 `_internal` 文件夹里，**启动无延迟**。

### ✨ 主要特性
*   **动态映射 (Dynamic Mapping)**: 借鉴了 PyStand 的设计。你无需修改源码。只要把启动器重命名为 `A.exe`，它就会自动去启动 `_internal\A.exe`。
*   **极致轻量**: 纯 C++ WinAPI 编写，编译后仅 50KB 左右。
*   **无依赖**: 客户端无需安装 VC++ 运行库即可运行。
*   **Unicode 支持**: 完美支持包含中文路径的运行环境。

### 🚀 使用方法

#### 1. 下载启动器
进入本仓库的 [Actions](../../actions) 页面，点击最新的一次构建，下载 `Shim.exe`。

#### 2. 准备 Nuitka 打包文件
正常使用 Nuitka 的独立模式（standalone）打包你的 Python 项目：
```bash
python -m nuitka --standalone --output-dir=dist myscript.py
```

#### 3. 整理目录结构
1.  将 Nuitka 生成的文件夹（例如 `myscript.dist`）重命名为 **`_internal`**。
2.  将下载的 `Shim.exe` 放到 `_internal` 的同级目录下。
3.  **重命名 `Shim.exe`**，使其名字与 `_internal` 里面的 exe 名字一致。

**示例：**
如果你的内部程序是 `_internal\monitor.exe`，请把外面的 `Shim.exe` 改名为 `monitor.exe`。

#### 📁 目录结构对比

**使用前 (乱糟糟):**
```text
MyProgram/
├── myscript.exe
├── python310.dll
├── libcrypto.dll
├── vcruntime140.dll
├── ... (几十个文件铺在根目录)
```

**使用后 (清爽):**
```text
MyProgram/
├── myscript.exe         <-- 这是改名后的启动器 (用户点击这个)
└── _internal/           <-- 真正的程序和依赖都藏在这里
    ├── myscript.exe     <-- Nuitka 生成的原程序
    ├── python310.dll
    └── ...
```

### 🛠️ 自定义图标
如果你想修改启动器的图标：
1. Fork 本仓库。
2. 替换根目录下的 `appicon.ico` 文件。
3. GitHub Actions 会自动为你编译带新图标的启动器。

---

### License
MIT License
