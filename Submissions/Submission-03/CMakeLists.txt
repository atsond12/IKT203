# CMakeList.txt : CMake project for Submission-01, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (Submission-03 "main.cpp" "main.h")
target_link_libraries(Submission-03 PRIVATE LibExample)

if (CMAKE_VERSION VERSION_GREATER 3.20)
  set_property(TARGET Submission-03 PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
