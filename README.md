# Ballistix Injector (v1.0)

A lightweight, modern DLL injector built with C++ and ImGui utilizing a sleek animated interface on top of ImGui Viewports.

![Preview](https://your-image-link-or-gif-here.com)

## ✨ Features
* **Modern UI**: Full multi-viewport integration with custom dark theme styling.
* **Animations**: Fluid Ease In-Out animations for the process list and logging console.
* **Clean Logging**: Real-time structured output tracking with injection phase breakdown.
* **Registry Config**: Component states, colors, and speeds are stored directly in the Windows Registry.
* **CMake Architecture**: Easily buildable across multiple toolchains.

## 🛠️ Injection Methods
* [x] **CreateRemoteThread** (Standard WinAPI thread invocation)
* [ ] **LdrLoadDll Stub** (Coming soon in v1.1)
* [ ] **Manual Mapping** (Coming soon in v1.2)

## 🚀 Building from Source
Prerequisites: CMake and a C++17 compatible compiler (MSVC recommended).

```bash
mkdir out && cd out
cmake ..
cmake --build . --config Release
```
