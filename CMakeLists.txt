cmake_minimum_required (VERSION 3.8)
set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_DIR})

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("KiwiInjector")
find_package(imgui CONFIG REQUIRED)
find_package(minhook CONFIG REQUIRED)
set(COMMON_LIBS imgui::imgui d3d11 dxgi winmm)  
add_executable(KiwiInjector WIN32 "src/KiwiInjector.cpp" "src/gui.cpp" "src/logic.cpp" "headers/KiwiInjector.h")
target_link_libraries(KiwiInjector PRIVATE ${COMMON_LIBS})
add_compile_options(/EHsc)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET KiwiInjector PROPERTY CXX_STANDARD 20)
endif()

