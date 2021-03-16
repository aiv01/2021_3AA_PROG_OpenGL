# OpenGL Playground
Project for teaching Hardware Accelerated Graphics with OpenGL

# Project Setup 
This is a CMake project with Conan integration so the following clients need to be installed needed on the workstation:
* cmake: https://cmake.org/
* conan: https://conan.io/
* python: https://www.python.org/ (needed by conan client)

# VSCode Users
For VSCode users, exists a bug on the intellisense of the C/C++ plugin when importing libraries with with cmake/conan, making the headers files not resolvables. This happen when cmake try to handle intellisense configuration in these two ways:

```javascript
//In .vscode/settings.json
"C_Cpp.default.configurationProvider": "ms-vscode.cmake-tools"
```
or 
```javascript
//In .vscode/c_cpp_properties.json
"configurationProvider": "ms-vscode.cmake-tools"
```

> Bug Note: https://github.com/microsoft/vscode-cmake-tools/issues/1213

Here a possibile workaround:

1. Delete *configurationProvider* from ```settings.json``` and/or from ```c_cpp_properties.json```
1. Add to C/C++ Plugin include path, the repo where cmake/conan install dependencies the include path:
    * on Windows: ```~/.conan/data/**```
    * also add: ```${workspaceFolder}/include``` (because is cmake is no more used to setup the c++ plugin)
