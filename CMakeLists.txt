# Set the minimum version of CMake required to build this project.
# This ensures that older versions of CMake don't try to run with features they don't understand.
cmake_minimum_required(VERSION 3.20)

# Define the project name. This will be the top-level name in Visual Studio.
# It also enables the C++ language (CXX).
project(IKT203_Course_Assignments LANGUAGES CXX)

# We enforce C++17 (or C++20 if you prefer) for the entire project.
# All targets (exercises, libraries, etc.) will inherit this setting.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
  add_compile_options("/Zc:__cplusplus")
endif()

# This is the core of the orchestration. CMake will now step into each of these
# folders and process their own CMakeLists.txt files.
# The order matters here: we add LibExample first so that its library is
# defined before the executables that need to link to it.

add_subdirectory(LibExample)
add_subdirectory(Submissions)
add_subdirectory(Portfolio)

# --- End of File ---