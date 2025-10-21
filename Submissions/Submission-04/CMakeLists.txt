# CMakeList.txt : CMake project for Submission-01, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (Submission-04 "main.cpp" "main.h" "BankAccount.h" "ReadNames.cpp" "ReadNames.h"  "BankAccount.cpp" "BankAccount.h" "BankAccountList.cpp" "BankAccountList.h")
target_link_libraries(Submission-04 PRIVATE SharedCode)

# Add source files to a Submission04Lib library
add_library(Submission04Lib
    BankAccount.cpp
    BankAccount.h
    ReadNames.cpp
    ReadNames.h
    BankAccountList.cpp
    BankAccountList.h
)

target_include_directories(Submission04Lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if (CMAKE_VERSION VERSION_GREATER 3.20)
  set_property(TARGET Submission-03 PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
