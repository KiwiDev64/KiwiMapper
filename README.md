
# Kiwi Injector (v1.0)

A lightweight, modern DLL injector built with C++ and ImGui utilizing a sleek animated interface on top of ImGui Viewports.

![Preview](https://github.com/user-attachments/assets/2cfc5901-7824-4672-8723-4472faca76e2)

## ✨ Features
* **Modern UI**: Full multi-viewport integration with custom dark theme styling.
* **Animations**: Fluid Ease In-Out animations for the process list and logging console.
* **Clean Logging**: Real-time structured output tracking with injection phase breakdown.
* **Registry Config**: Component states, colors, and speeds are stored directly in the Windows Registry. *(coming soon)*
* **CMake Architecture**: Easily buildable across multiple toolchains.

## 🛠️ Injection Methods
* [x] **LoadLibraryA Inject** (Standard WinAPI thread invocation)
* [ ] **LdrLoadDll Inject** (Coming soon in v1.1)
* [ ] **Manual Mapping** (Coming soon in v1.2)
* [ ] **Registry Config** (Coming soon in v1.2)

## 🚀 Building from Source
Prerequisites: CMake and a C++20 compatible compiler (MSVC recommended).

```bash
mkdir out && cd out
cmake ..
cmake --build . --config Release
```
