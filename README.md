# KiwiMapper (v1.0)

A lightweight, modern dynamic memory mapper and module runner built with C++20 and ImGui. Features a sleek, responsive interface leveraging ImGui Viewports for real-time process monitoring.

![Preview](https://github.com)

## ✨ Features
* **Modern UI**: Full multi-viewport integration with custom dark theme styling.
* **Fluid Animations**: Smooth Ease In-Out animations for the process tree and runtime console.
* **Structured Logs**: Real-time diagnostic output tracking module mapping phases.
* **Registry Config**: Save window layouts, animation speeds, and theme preferences directly to the Windows Registry. *(coming soon)*
* **CMake Architecture**: Easily buildable across multiple toolchains.

## 🛠️ Memory Mapping & Execution Methods
* [x] **Standard Thread Mapping** (`LoadLibraryA` invocation via WinAPI)
* [ ] **Subsystem Module Mapping** (`LdrLoadDll` execution pathing) *(Coming soon in v1.1)*
* [ ] **Manual PE Mapping & Relocation** (Custom virtual memory allocation) *(Coming soon in v1.2)*
* [ ] **Virtual Memory Section Mapping** (`NtMapViewOfSection` implementation) *(Coming soon in v1.3)*

## 🚀 Building from Source
Prerequisites: CMake and a C++20 compatible compiler (MSVC recommended).

```bash
mkdir out && cd out
cmake ..
cmake --build . --config Release
```
