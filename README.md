# godot_kinect
Godot module to control Microsoft Kinect v1. This module it's only compatible with Windows, and it was test in Windows 10 pro (x64).

## Prerequisites
First of all, you need to install the drivers for kinect v1 "https://www.microsoft.com/en-us/download/details.aspx?id=40278". Also you need to be able to compile with SCons and MSVC

### Enviroment variables
The SConstruct file need some environment variables:
- CPP_BINDINGS (directory of godot-cpp)
- GODOT_HEADERS (directory of godot-headers)
- KINECT_HEADERS (directory of kinect headers. By default, "C:\Program Files\Microsoft SDKs\Kinect\v1.8\inc")
- KINECT_LIB (directory of kinect lib. By default, "C:\Program Files\Microsoft SDKs\Kinect\v1.8\lib")

## Screenshots
### Main window
![main_window](/screenshots/screenshot_1.png)